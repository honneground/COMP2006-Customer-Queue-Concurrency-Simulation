#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define TELLERS 3       // Number of tellers
#define OUTFILE "r_log" // Output log file name
#define INFILE "c_file" // Input customer file name

// Structure representing a customer
typedef struct {
    int number;         // Customer's number
    int serve_time;     // Time taken to serve the customer
    char arrival_time[9];  // Customer's arrival time log
} Customer;

// Structure representing the customers queue
typedef struct {
    Customer** array;   // Array of pointers to customers
    int size;           // Maximum size of the queue
    int count;          // Current number of customers in the queue
    int head;           // Index of the front customer in the queue
    int tail;           // Index of the last customer in the queue
    int finished;       // Flag indicating whether the queue is finished
} Queue;

// Function prototypes
void* teller(void*);
void* customer();
void assign_time(char *timeStr);

Queue* queue_init(int size);
int queue_enqueue(Queue* q, Customer* customer);
Customer* queue_dequeue(Queue* q);
int queue_is_empty(Queue* q);
int queue_is_full(Queue* q);
int queue_is_finished(Queue* q);
void queue_mark_finished(Queue* q);
void queue_destroy(Queue* q);

FILE* output_file;  // Pointer to output file
FILE* input_file;   // Pointer to input file
Queue* c_queue;     // Pointer to customer queue
pthread_mutex_t mutex_queue;  // Mutex for synchronizing access to the queue
pthread_mutex_t mutex_output; // Mutex for synchronizing file writing
pthread_cond_t cond_full;     // Condition variable for queue full
pthread_cond_t cond_empty;    // Condition variable for queue empty

// Global variables in shared memory
int terminated_teller_count;  // Counter for terminated tellers
int periodic_time;            // Time interval for customer arrival
int served_count[3] = {0, 0, 0}; // Count of customers served by each teller


/**
 * Entry point for the bank simulation program.
 * 
 * The main function initializes the necessary resources, creates threads for 
 * customers and tellers, and starts the bank simulation. It ensures that all threads 
 * finish their tasks before cleaning up the resources and ending the program.
 */
int main(int argc, char* argv[]){
    pthread_t teller_threads[TELLERS]; 
    pthread_t customer_thread;
    int teller_numbers[TELLERS];
    int length_of_cq;
    int i;

    printf("Program initiating...\n");
    
    // Validate command line arguments
    if(argc != 3){
        printf("Invalid numbers of command line arguments.\nUsage: ./cq m t\n(m is the length of customer queue, t is the periodic time for the customer to arrive at the queue.)\n");
        return 1;
    }
    length_of_cq = atoi(argv[1]);
    periodic_time = atoi(argv[2]);
    if(length_of_cq < 1 || periodic_time < 0){
        printf("Command line arguments invalid.\n");
        if(length_of_cq < 1) printf("Length of customer must be an integer that is equal or greater than 1.\n");
        if(periodic_time < 0) printf("Periodic time must be an integer that is equal or greater than 0.\n");
        return 1;
    }

    // Open input and output files
    input_file = fopen(INFILE, "r");
    if(input_file == NULL){
        perror("Fail to open the input file. Please check that the \"c_file\" file is generated and accessible by the program.");
        return 1;
    }
    output_file = fopen(OUTFILE, "w");
    if(output_file == NULL){
        perror("Fail to access the output file.");
        return 1;
    }

    // Initialize customer queue
    c_queue = queue_init(length_of_cq);

    // Initialize mutexes and condition variables
    pthread_mutex_init(&mutex_output, NULL);
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_cond_init (&cond_full, NULL);
    pthread_cond_init (&cond_empty, NULL);
    
    printf("Creating threads...\n");
    
    // Create customer thread
    if(pthread_create(&customer_thread, NULL, &customer, NULL) != 0){
        perror("Fail to create the customer thread.");
        return 1;
    }

    // Create teller threads
    for(i = 0; i < TELLERS; i++){
        teller_numbers[i] = i + 1;

        if(pthread_create(&teller_threads[i], NULL, &teller, (void*)&teller_numbers[i]) != 0){
            perror("Fail to create the teller thread");
            return 1;
        }
    }
    printf("Threads executing...\n");
    
    // Wait for customer thread to finish
    pthread_join(customer_thread, NULL);
    printf("Customer thread finished. Wait for the finish of the last shift...\n");
    
    // Wait for all teller threads to finish
    for(i = 0; i < TELLERS; i++) pthread_join(teller_threads[i], NULL);
    
    printf("The last teller terminated. Now deallocating memories...\nPlease refer the outfile to the \"r_log\" file.\n");

    // Clean up resources
    fclose(output_file);
    queue_destroy(c_queue);
    pthread_mutex_destroy(&mutex_output);
    pthread_mutex_destroy(&mutex_queue);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_full);
    
    return 0;
}

/**
 * Represents the customer serving queue in the system.
 * 
 * This function reads from the input file to extract information about the customer
 * such as their number and service time. It then enqueues the customer to the shared 
 * queue and signals the tellers. For synchronization, this function makes use of 
 * mutexes and condition variables.
 */
void* customer(){
    // Continuously read from the file until EOF is reached
    while(!feof(input_file)){
        // Allocate memory for a new customer
        Customer* new_customer = (Customer*)malloc(sizeof(Customer));
        
        // Try to read customer details from the file
        if(fscanf(input_file, "%d %d", &new_customer->number, &new_customer->serve_time) != 2){
            // If reading was unsuccessful, free the allocated memory and skip to the next line
            free(new_customer);
        } else if(new_customer->number < 1 || new_customer->serve_time < 1 || new_customer->serve_time > 10){
            // If input format is incorrect or out of allowed range, print an error and exit
            perror("Invalid format detected in the input file. Notice that the customer number should be an integer increment from 1, and the service time should be an integer between 1 and 10.");
            free(new_customer);
            queue_mark_finished(c_queue);
            fclose(input_file);
            pthread_exit(0);
        } else {
            // Lock the queue mutex to ensure exclusive access
            pthread_mutex_lock(&mutex_queue);

            // Wait until there's space in the queue
            if(queue_is_full(c_queue)) pthread_cond_wait(&cond_full, &mutex_queue);
            
            // Assign current time as the arrival time for the customer
            assign_time(new_customer->arrival_time);
            // Enqueue the customer to the queue
            queue_enqueue(c_queue, new_customer);

            // Release the mutex lock
            pthread_mutex_unlock(&mutex_queue);
            // Signal the condition variable to indicate there's a new customer in the queue
            pthread_cond_signal(&cond_empty);
            
            // Lock the output mutex to ensure exclusive access to the output file
            pthread_mutex_lock(&mutex_output);

            // Print customer details to the output file
            fprintf(output_file, "-----------------------------------------------------------------------\n");
            fprintf(output_file, "%d: %d\n",new_customer->number, new_customer->serve_time);
            fprintf(output_file, "Arrival Time: %s\n", new_customer->arrival_time);
            fprintf(output_file, "------------------------------------------------------------------------\n\n");

            // Release the output mutex
            pthread_mutex_unlock(&mutex_output);
            
            // Sleep for a predefined periodic time before reading the next customer
            sleep(periodic_time);
        }
    }
    
    // Once all customers have been read, mark the queue as finished
    pthread_mutex_lock(&mutex_queue);
    queue_mark_finished(c_queue);
    pthread_mutex_unlock(&mutex_queue);
    
    // Broadcast to all waiting tellers that the queue has been marked finished
    pthread_cond_broadcast(&cond_empty);

    // Close the input file
    fclose(input_file);
    pthread_exit(0);
}

/**
 * Function representing the teller's behavior.
 * 
 * This function simulates the behavior of a teller. It processes customers in the queue, logs 
 * the activity to the output file, and then handles termination of the teller.
 * 
 * @param teller_no_prt Pointer to the teller's number.
 * @return NULL.
 */
void* teller(void* teller_no_prt){
    int teller_number;
    char buffer_time[9];       // Buffer to store various timestamps
    char start_time[9];        // Time when the teller thread started
    Customer* current_customer;
    
    assign_time(start_time);   // Get the current time
    teller_number = *((int*)teller_no_prt);

    while(1)
    {
        // Lock the queue mutex and wait if the queue is empty and not finished
        pthread_mutex_lock(&mutex_queue);
        while(queue_is_empty(c_queue) && !queue_is_finished(c_queue)) pthread_cond_wait(&cond_empty, &mutex_queue);
        
        // If queue is empty and finished, release mutex and terminate the thread
        if(queue_is_empty(c_queue) && queue_is_finished(c_queue)){
            pthread_mutex_unlock(&mutex_queue);
            break; 
        }
        
        // Dequeue a customer and unlock the mutex
        current_customer = queue_dequeue(c_queue);
        pthread_mutex_unlock(&mutex_queue);
        pthread_cond_signal(&cond_full); // Signal that queue has space now

        assign_time(buffer_time);
        
        // Lock the output mutex and log customer response time
        pthread_mutex_lock(&mutex_output);
        fprintf(output_file, "Teller: %d\n", teller_number);
        fprintf(output_file, "Customer: %d\n", current_customer->number);
        fprintf(output_file, "Arrival time: %s\n", current_customer->arrival_time);
        fprintf(output_file, "Response Time: %s\n\n", buffer_time);
        pthread_mutex_unlock(&mutex_output);

        // Simulate the time taken to serve the customer
        sleep(current_customer->serve_time);

        assign_time(buffer_time);
        
        // Lock the output mutex and log customer completion time
        pthread_mutex_lock(&mutex_output);
        fprintf(output_file, "Teller: %d\n", teller_number);
        fprintf(output_file, "Customer: %d\n", current_customer->number);
        fprintf(output_file, "Arrival time: %s\n", current_customer->arrival_time);
        fprintf(output_file, "Completion Time: %s\n\n", buffer_time);
        pthread_mutex_unlock(&mutex_output);

        // Deallocate memory for processed customer
        free(current_customer);
        served_count[teller_number-1]++;
    }

    //Set the terminated time, before requesting writing to the log file
    assign_time(buffer_time);
    
    // Lock the output mutex and log teller termination details
    pthread_mutex_lock(&mutex_output);
    terminated_teller_count += 1;
    fprintf(output_file, "Termination: teller-%d\n", teller_number);
    fprintf(output_file, "Served customers: %d\n", served_count[teller_number-1]);
    fprintf(output_file, "Start time: %s\n", start_time);
    fprintf(output_file, "Termination Time: %s\n\n", buffer_time);
    pthread_mutex_unlock(&mutex_output);
    
    // The last teller terminated logs statistics for all
    if(terminated_teller_count == TELLERS){
        fprintf(output_file, "Teller Statistic\n");
        int total_served_count = 0;
        for(int i=0; i<TELLERS; i++){
            fprintf(output_file, "Teller-%d serves %d customers.\n", i+1, served_count[i]);
            total_served_count += served_count[i];
        }
        fprintf(output_file, "Total number of customers: %d customers.\n", total_served_count);
    }   
    pthread_exit(0);
}

/**
 * Initializes and allocates memory for a queue with a given size.
 * 
 * @param size The size of the queue to initialize.
 * @return Pointer to the initialized queue.
 */
Queue* queue_init(int size){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->array = (Customer**)malloc(sizeof(Customer*) * size);
    q->size = size;
    q->count = 0;
    q->head = 0;
    q->tail = 0;
    q->finished = 0;
    return q;
}

/**
 * Checks if a queue is empty.
 * 
 * @param q Pointer to the queue to check.
 * @return 1 if the queue is empty, otherwise 0.
 */
int queue_is_empty(Queue* q){
    return q->count == 0;
}

/**
 * Checks if a queue is full.
 * 
 * @param q Pointer to the queue to check.
 * @return 1 if the queue is full, otherwise 0.
 */
int queue_is_full(Queue* q){
    return q->count == q->size;
}

/**
 * Enqueues a customer into the queue.
 * 
 * @param q Pointer to the queue to enqueue into.
 * @param customer Pointer to the customer to enqueue.
 * @return 1 if enqueued successfully, 0 if the queue is full.
 */
int queue_enqueue(Queue* q, Customer* customer){
    if(queue_is_full(q)){
        return 0;  // Queue is full
    }
    q->array[q->tail] = customer;
    q->count++;
    q->tail = (q->tail + 1) % q->size;
    return 1;
}

/**
 * Dequeues a customer from the queue.
 * 
 * @param q Pointer to the queue to dequeue from.
 * @return Pointer to the dequeued customer or NULL if the queue is empty.
 */
Customer* queue_dequeue(Queue* q){
    if(queue_is_empty(q)){
        return NULL;
    }
    Customer* customer = q->array[q->head];
    q->count--;
    q->head = (q->head + 1) % q->size;
    return customer;
}

/**
 * Deallocates memory for a queue and its customers.
 * 
 * @param q Pointer to the queue to destroy.
 */
void queue_destroy(Queue* q){
    for (int i = 0; i < q->count; i++){
        free(q->array[i]);
    }
    free(q->array);
    free(q);
}

/**
 * Checks if queue processing has finished.
 * 
 * @param q Pointer to the queue to check.
 * @return 1 if the queue is marked as finished, otherwise 0.
 */
int queue_is_finished(Queue* q){
    return q->finished;
}

/**
 * Marks a queue as finished.
 * 
 * @param q Pointer to the queue to mark.
 */
void queue_mark_finished(Queue* q){
    q->finished = 1;
}

/**
 * Assigns the current time to a given string address in the format HH:MM:SS.
 * 
 * @param time_string_address Pointer to the character buffer to store the formatted time.
 */
void assign_time(char *time_string_address){
    time_t raw_time;
    struct tm *time_string_out;

    time(&raw_time);
    time_string_out = localtime(&raw_time);
    
    strftime(time_string_address, 9, "%H:%M:%S", time_string_out);
}