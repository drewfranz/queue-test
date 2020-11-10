//
//  QueueTest.cpp
//
//  Created by Drew Franz on 11/06/2020
//  Copyright (c) 2020 Drew Franz. All rights reserved.
//
#include <iostream>
#include <mach/mach.h>
#include <gtest/gtest.h>
#include "Queue.h"

using namespace std;

namespace {

struct task_basic_info t_info;

class Queue : public ::testing::Test {
protected:
    int initialMemoryUsage;
    static const int QUEUES_COUNT = 15;
    const int BYTE_MAX = 2048;
    Q* queuesArray[QUEUES_COUNT];
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

public:
    virtual int GetMemoryUsage() {
        if (KERN_SUCCESS != task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count)) {
            return -1;
        }

        return t_info.resident_size;
    }

    virtual void CreateQueuesArray() {
         for (size_t i = 0; i < QUEUES_COUNT; i++) {
             queuesArray[i] = create_queue();
         }
    }

    virtual void DestroyQueuesInArray() {
         for (size_t i = 0; i < QUEUES_COUNT; i++) {
             if (queuesArray[i] != nullptr) {
                destroy_queue(queuesArray[i]);
             }
         }
    }

    virtual int fillQueuesWithDataAndGetFirstByteOffset(int count) {
        // Seed the random number generator.
        srand((unsigned) time(0));

        // Set some initial values
        const int MAX_RAND = 85;
        const int RAND_MOD = 10;
        const int BYTE_OFFSET = 35;
        int randomByteCount;

        // Fill each queue with between 75 & 85 bytes.
        for (size_t i = 0; i < count; i++) {
            randomByteCount = MAX_RAND - (rand() % RAND_MOD);

            for (size_t j = 0; j <= randomByteCount; j++) {
                // Cast 'j' as an int we can use and offset the byte so we get into more standard chars.
                unsigned int newJ = j + BYTE_OFFSET;
                // Cast to a char byte. 
                unsigned char byteToEnqueue = static_cast<char>(newJ);
                // Add the byte to the queue
                enqueue_byte(queuesArray[i], byteToEnqueue);
            }
        }

        return BYTE_OFFSET;
    }
};

class QueueTest : public Queue {

};

// Test that a queue object did get created.
TEST_F(QueueTest, IsQueueHandlerInitialized) {
    // Set the memory used value before running the first test.
    initialMemoryUsage = GetMemoryUsage();

    // Create our first queue
    Q* queue = create_queue();
    // Check the queue was created.
    ASSERT_NE(queue, nullptr);
    // Destroy the queue.
    destroy_queue(queue);
}

// // Test the ability to add a byte to an existing queue and remove that same byte immediately.
TEST_F(QueueTest, AddNewByteToExistingQueue) {
    Q* queue = create_queue();
    // The byte to add.
    unsigned char byteToAddToTheQueue = 0;
    enqueue_byte(queue, byteToAddToTheQueue);

    // The returned byte from the queue.
    unsigned char byteDequeued = dequeue_byte(queue);

    // Check that the returned byte is equal to the one we added.
    ASSERT_EQ(byteDequeued, byteToAddToTheQueue);
    // Destroy the queue.
    destroy_queue(queue);
}

// Test the ability to create 15 queues at once.
TEST_F(QueueTest, TestThat15QueuesWereCreated) {
    // Create an array of queue object handlers.
    CreateQueuesArray();

    // Test that the last slot in the queues array is not null.
    for (size_t i = 0; i <= (QUEUES_COUNT - 1); i++) {
        EXPECT_TRUE(queuesArray[i] != nullptr);
    }
    
    // Destroy the queue handlers.
    DestroyQueuesInArray();
}

// Test that each queue can support ~80 bytes each.
TEST_F(QueueTest, AddApproximately80BytesEach) {
    // First create all of the queue object handlers.
    CreateQueuesArray();
    // Populate the queues with data.
    int byteOffset = fillQueuesWithDataAndGetFirstByteOffset(QUEUES_COUNT);

    for (size_t i = 0; i < QUEUES_COUNT; i++) {
        // Grab the first byte from the queue.
        unsigned char dequeuedByte = dequeue_byte(queuesArray[i]);
        // Get the char we need to check against.
        unsigned char byteToCheck = static_cast<char>(byteOffset);
        // Test the equality
        ASSERT_EQ(dequeuedByte, byteToCheck);
    }

    // Finally, destroy all queues.
    DestroyQueuesInArray();
}

TEST_F(QueueTest, TestThatQueuesStayWithinMemoryUsage) {
    // First create all of the queue object handlers.
    CreateQueuesArray();
    // Populate the queues with data.
    int byteOffset = fillQueuesWithDataAndGetFirstByteOffset(QUEUES_COUNT);

    // Get the total memory used.
    int finalMemoryUsage = GetMemoryUsage();
    int totalMemoryUsed = finalMemoryUsage - initialMemoryUsage;
    
    ASSERT_EQ(totalMemoryUsed, BYTE_MAX);
    // Finally, destroy all queues.
    DestroyQueuesInArray();
}
TEST_F(QueueTest, TestIfOutOfMemoryHasBeenCalled) {
    // First create all of the queue object handlers.
    CreateQueuesArray();
    // Populate the queues with data.
    // int b = fillQueuesWithDataAndGetFirstByteOffset(QUEUES_COUNT + 100);
    
    EXPECT_ANY_THROW(fillQueuesWithDataAndGetFirstByteOffset(QUEUES_COUNT + 100));
    // Finally, destroy all queues.
    DestroyQueuesInArray();
}

TEST_F(QueueTest, TestIfIllegalOperationHasBeenCalled) {
    EXPECT_ANY_THROW(destroy_queue(NULL));
}
} // namespace