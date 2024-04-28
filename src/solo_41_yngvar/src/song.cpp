#include "song/song.h"
#include "kernel/pit.h"
#include "libc/ports.h"
#include "libc/stdio.h"

// Enable PC speaker
void enable_speaker(){
    // Read state of PC speaker control register
    uint8_t speaker_state = port_byte_in(PC_SPEAKER_PORT);
    // Enable speaker by setting bits 0 and 1 to 1, rest are set to 0 (3 = 00000011)
    port_byte_out(PC_SPEAKER_PORT, speaker_state | 3);
}

// Disable PC speaker
void disable_speaker() {
    // Read state of PC speaker control register
    uint8_t speaker_state = port_byte_in(PC_SPEAKER_PORT);
    // Disable speaker by setting bits 0 and 1 to 0, rest are set to 1
    port_byte_out(PC_SPEAKER_PORT, speaker_state & 0xFC);
}

// Implementation of how to play song
void play_song_impl(Song *song) {
    // Loop through the notes
    for (uint32_t i = 0; i < (song->length); i++) {
        // Play each note's frequency for duration
        play_sound(song->notes[i].frequency, song->notes[i].duration);
    }
}

// Play song
void play_song(Song *song) {
    play_song_impl(song); // Call implementation
}

// Play tone
void play_sound(uint32_t frequency, uint32_t duration) {
    // If frequency is not 0 to avoid division by 0, or keep silent
    if (frequency != 0) {
        enable_speaker(); // Enable speaker
        uint16_t divisor = (uint16_t)(1193180 / frequency); // Set divisor
        port_byte_out(PIT_CMD_PORT, 0xB6); // Set to square mode on channel 2
        port_byte_out(PIT_CHANNEL2_PORT, divisor & 0xFF); // Send low byte to data port
        port_byte_out(PIT_CHANNEL2_PORT, (divisor >> 8)); // Send high byte to data port
        sleep_interrupt(duration); // CPU sleep while tone is played
        disable_speaker(); // Stop sound output
    }
}

// Create new song player object
SongPlayer* create_song_player() {
    auto* player = new SongPlayer(); // Dynamically allocate new object
    player->play_song = play_song_impl; // Set function pointer
    return player; // Return pointer
}
