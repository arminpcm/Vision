// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <cstring>
#include <stdexcept>

#include "Library/SharedMemory/ChannelHeader.hpp"

template <typename MessageType>
class Publisher {
private:
    int shm_id_;          // Shared memory ID
    void* shared_memory_; // Pointer to the shared memory

public:
    Publisher(key_t key, size_t capacity_);
    ~Publisher();

    void Init();

    void Publish(const MessageType& message);
};

template <typename MessageType>
Publisher<MessageType>::Publisher(key_t key, size_t capacity_)
    : shared_memory_Key(key) {
    SetUp();
}

template <typename MessageType>
Publisher<MessageType>::~Publisher() {
    // Detach from shared memory
    shmdt(shared_memory_);
}

template <typename MessageType>
void Publisher<MessageType>::Init() {
    // Create or attach to shared memory segment and initialize it

    // Try to create the shared memory segment
    shm_id_ = shmget(shared_memory_Key, sizeof(ChannelHeader<MessageType>) + sizeof(MessageType) * capacity_, IPC_CREAT | IPC_EXCL | 0666);

    if (shm_id_ == -1) {
        // If it already exists, throw an exception
        if (errno == EEXIST) {
            throw std::runtime_error("Shared memory segment already exists.");
        } else {
            perror("shmget");
            exit(1);
        }
    }

    // Attach to shared memory
    shared_memory_ = shmat(shm_id_, NULL, 0);
    if (shared_memory_ == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    // Initialize the ChannelHeader if it's a new segment
    ChannelHeader<MessageType>* header = static_cast<ChannelHeader<MessageType>*>(shared_memory_);
    if (header->capacity_ == 0) {
        header->capacity_ = capacity_;
        header->start_ = 0;
        header->end_ = 0;
        header->size_ = 0;
        pthread_mutex_init(&(header->writer_mutex_), NULL);
    }
}

template <typename MessageType>
void Publisher<MessageType>::Publish(const MessageType& message) {
    // Lock the writer mutex to ensure exclusive access
    ChannelHeader<MessageType>* header = static_cast<ChannelHeader<MessageType>*>(shared_memory_);
    pthread_mutex_lock(&(header->writer_mutex_));

    // Calculate the offset for the new data
    char* offset = static_cast<char*>(shared_memory_) + sizeof(ChannelHeader<MessageType>) + header->end_ * sizeof(MessageType);

    // Serialize and copy the message
    message.SerializeToArray(offset, sizeof(MessageType));

    // Update circular buffer pointers
    header->end_ = (header->end_ + 1) % header->capacity_;

    if (header->size_ < header->capacity_) {
        header->size_++;
    } else {
        // Error: Buffer is full, print an error message
        std::cerr << "Error: Buffer is full. Overwriting data." << std::end_l;
        // Update the start_ pointer to overwrite the oldest data
        header->start_ = (header->start_ + 1) % header->capacity_;
    }

    // Unlock the writer mutex
    pthread_mutex_unlock(&(header->writer_mutex_));
}
