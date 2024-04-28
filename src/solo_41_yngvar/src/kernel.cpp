#include "libc/stdio.h"
#include "kernel/memory.h"
#include "kernel/pit.h"
#include "song/song.h"


// Existing global operator new overloads
void* operator new(size_t size) {
return malloc(size);
}

void* operator new[](size_t size) {
return malloc(size);
}

// Existing global operator delete overloads
void operator delete(void* ptr) noexcept {
free(ptr);
}

void operator delete[](void* ptr) noexcept {
free(ptr);
}

// Add sized-deallocation functions
void operator delete(void* ptr, size_t size) noexcept {
(void)size; // Size parameter is unused, added to match required signature
free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
(void)size; // Size parameter is unused, added to match required signature
free(ptr);
}

extern "C" int kernel_main(void);

int kernel_main(){

    // Allocate and deallocate some memory using the kernel memory manager
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    delete memory2;
    print("Deleted memory allocation of 54321 bytes\n");
    char* memory4 = new char[1000]();
    
    // Showcasing sleep functions
    print("[%d]: Sleeping 1000ms with busy-waiting (HIGH CPU).\n", get_current_tick());
    sleep_busy(1000);
    print("[%d]: Slept using busy-waiting.\n", get_current_tick());
    print("[%d]: Sleeping 1000ms with interrupts (LOW CPU).\n", get_current_tick());
    sleep_interrupt(1000);
    print("[%d]: Slept using interrupts.\n", get_current_tick());
    
    // Initialize song
    Song* songs[] = {new Song(music_7, sizeof(music_7) / sizeof(Note))};
    
    // Initialize music player
    SongPlayer* player = create_song_player();

    // Play song
    print("Playing Song...\n");
    if (!player) { // Error handling
        print("Error: Player not initialized.\n");
    } else {
        if (songs[0]) {  // Check if the song was successfully created
            player->play_song(songs[0]); // Play song 0
            print("Finished playing the song.\n");
        } else { // Error handling
            print("Error: Failed to create song.\n");
        }
    }

    // Clean up
    delete player; // Delete music player
    for (auto &song : songs) {
        delete song;  // Deleting each dynamically allocated Song
    }

    // Demonstrate key logger
    print("Keyboard logger:");
    print("\n");

    // Running loop
    while(1) {
    }
    

    return 0;
}