#include "cp437_transcode.h"

// UTF-8 decoder state
static uint32_t current_codepoint = 0;
static uint8_t  bytes_needed = 0;

// Escape sequence bypass state (to prevent mangling ESC/P or IBM control parameters)
static uint8_t  esc_state = 0;

void cp437_reset() {
    current_codepoint = 0;
    bytes_needed = 0;
    esc_state = 0;
}

// Maps a 16-bit Unicode codepoint to IBM CP437 byte (Table 3.2 in DMP3000 manual)
static uint8_t unicode_to_cp437(uint32_t cp) {
    if (cp < 0x80) {
        return (uint8_t)cp;
    }

    switch (cp) {
        // Latin-1 Supplement: Accented Vowels & Consonants
        case 0x00C7: return 128; // Ç
        case 0x00FC: return 129; // ü
        case 0x00E9: return 130; // é
        case 0x00E2: return 131; // â
        case 0x00E4: return 132; // ä
        case 0x00E0: return 133; // à
        case 0x00E5: return 134; // å
        case 0x00E7: return 135; // ç
        case 0x00EA: return 136; // ê
        case 0x00EB: return 137; // ë
        case 0x00E8: return 138; // è
        case 0x00EF: return 139; // ï
        case 0x00EE: return 140; // î
        case 0x00EC: return 141; // ì
        case 0x00C4: return 142; // Ä
        case 0x00C5: return 143; // Å
        case 0x00C9: return 144; // É
        case 0x00E6: return 145; // æ
        case 0x00C6: return 146; // Æ
        case 0x00F4: return 147; // ô
        case 0x00F6: return 148; // ö
        case 0x00F2: return 149; // ò
        case 0x00FB: return 150; // û
        case 0x00F9: return 151; // ù
        case 0x00FF: return 152; // ÿ
        case 0x00D6: return 153; // Ö
        case 0x00DC: return 154; // Ü
        case 0x00F8: return 155; // ø
        case 0x00A3: return 156; // £
        case 0x00D8: return 157; // Ø
        case 0x00D7: return 158; // ×
        case 0x0192: return 159; // ƒ
        case 0x00E1: return 160; // á
        case 0x00ED: return 161; // í
        case 0x00F3: return 162; // ó
        case 0x00FA: return 163; // ú
        case 0x00F1: return 164; // ñ
        case 0x00D1: return 165; // Ñ
        case 0x00AA: return 166; // ª
        case 0x00BA: return 167; // º
        case 0x00BF: return 168; // ¿
        case 0x00AC: return 170; // ¬
        case 0x00BD: return 171; // ½
        case 0x00BC: return 172; // ¼
        case 0x00A1: return 173; // ¡
        case 0x00AB: return 174; // «
        case 0x00BB: return 175; // »
        case 0x00DF: return 225; // ß
        case 0x00B5: return 230; // µ
        case 0x00B1: return 241; // ±
        case 0x00B0: return 248; // °
        case 0x00B7: return 250; // ·
        case 0x00B2: return 253; // ²

        // Box Drawing Characters (Unicode 0x2500 - 0x257F)
        case 0x2500: return 196; // ─
        case 0x2502: return 179; // │
        case 0x250C: return 218; // ┌
        case 0x2510: return 191; // ┐
        case 0x2514: return 192; // └
        case 0x2518: return 217; // ┘
        case 0x251C: return 195; // ├
        case 0x2524: return 180; // ┤
        case 0x252C: return 194; // ┬
        case 0x2534: return 193; // ┴
        case 0x253C: return 197; // ┼
        case 0x2550: return 205; // ═
        case 0x2551: return 186; // ║
        case 0x2554: return 201; // ╔
        case 0x2557: return 187; // ╗
        case 0x255A: return 200; // ╚
        case 0x255D: return 188; // ╝
        case 0x2560: return 204; // ╠
        case 0x2563: return 185; // ╣
        case 0x2566: return 203; // ╦
        case 0x2569: return 202; // ╩
        case 0x256C: return 206; // ╬

        // Block Elements
        case 0x2588: return 219; // █
        case 0x2580: return 223; // ▀
        case 0x2584: return 220; // ▄
        case 0x2591: return 176; // ░
        case 0x2592: return 177; // ▒
        case 0x2593: return 178; // ▓

        // Math & Currency Fallbacks
        case 0x20AC: return 'E'; // Euro sign fallback
        case 0x00A9: return 'c'; // (c)
        case 0x00AE: return 'r'; // (r)
        case 0x221E: return 236; // ∞
        case 0x2248: return 247; // ≈
        case 0x2264: return 243; // ≤
        case 0x2265: return 242; // ≥
        case 0x221A: return 251; // √

        // Capital vowels without direct CP437 accented glyphs -> map to base letter
        case 0x00C0: // À
        case 0x00C1: // Á
        case 0x00C2: // Â
        case 0x00C3: return 'A';
        case 0x00C8: // È
        case 0x00CA: // Ê
        case 0x00CB: return 'E';
        case 0x00CC: // Ì
        case 0x00CD: // Í
        case 0x00CE: // Î
        case 0x00CF: return 'I';
        case 0x00D2: // Ò
        case 0x00D3: // Ó
        case 0x00D4: // Ô
        case 0x00D5: return 'O';
        case 0x00D9: // Ù
        case 0x00DA: // Ú
        case 0x00DB: return 'U';

        default:
            return '?';
    }
}

int16_t cp437_process_byte(uint8_t b) {
    // Check if in ESC escape sequence
    if (esc_state > 0) {
        esc_state--;
        return b; // Pass raw escape sequence byte
    }
    if (b == 0x1B) { // ESC character
        esc_state = 2; // Pass following command and parameter bytes raw
        return b;
    }

    // Standard 7-bit ASCII
    if ((b & 0x80) == 0) {
        bytes_needed = 0;
        return b;
    }

    // UTF-8 lead byte detection
    if ((b & 0xE0) == 0xC0) {
        // 2-byte sequence
        current_codepoint = b & 0x1F;
        bytes_needed = 1;
        return 0; // Waiting for continuation byte
    } else if ((b & 0xF0) == 0xE0) {
        // 3-byte sequence
        current_codepoint = b & 0x0F;
        bytes_needed = 2;
        return 0;
    } else if ((b & 0xF8) == 0xF0) {
        // 4-byte sequence
        current_codepoint = b & 0x07;
        bytes_needed = 3;
        return 0;
    } else if ((b & 0xC0) == 0x80) {
        // Continuation byte
        if (bytes_needed > 0) {
            current_codepoint = (current_codepoint << 6) | (b & 0x3F);
            bytes_needed--;
            if (bytes_needed == 0) {
                // Completed codepoint
                return unicode_to_cp437(current_codepoint);
            }
            return 0; // More bytes required
        }
    }

    // Stray/invalid byte or already single CP437 byte (pass directly)
    bytes_needed = 0;
    return b;
}
