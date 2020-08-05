#include <iostream>
#include <termios.h>
#include <unistd.h>

unsigned addr { 0 };
unsigned value { 0 };
unsigned nibbles { 4 };
unsigned char buffer[1000];

int get() { return std::cin.get(); }
void put(int ch) { std::cout.put(ch); }
void put(const char *str) {
	while (*str) { put(*str++); }
}

void write_nibble(int nibble) {
	if (nibble >= 0) {
		if (nibble <= 9) {
			put('0' + nibble);
		} else if (nibble <= 15) {
			put('a' + nibble - 10);
		}
	}
}

bool valid_addr() {
	return addr < sizeof(buffer);
}

void write_addr() {
	write_nibble(addr >> 12);
	write_nibble((addr >> 8) & 0xf);
	write_nibble((addr >> 4) & 0xf);
	write_nibble(addr & 0xf);
	put(": ");
}

void clear() {
	++addr;
	value = 0;
	nibbles = 4;
	write_addr();
}


void add(char ch, int nibble) {
	if (nibbles == 6) {
		addr = value;
		nibbles = 2;
		put("\x1b[0E\x1b[2K");
		write_nibble(value >> 4);
		write_nibble(value & 0xf);
		value = 0;
	}
	if (nibbles < 4) {
		addr = (addr << 4) + nibble;
	} else {
		value = (value << 4 ) + nibble;
	}
	++nibbles;
	put(ch);
	if (nibbles == 4) { put(": "); }
}

int main() {
	class Terminal_Switcher {
			termios orig_;
		public:
			Terminal_Switcher() {
				tcgetattr(STDIN_FILENO, &orig_);
				termios raw { orig_ };
				raw.c_lflag &= ~ (ECHO | ICANON);
				tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
			}
			~Terminal_Switcher() {
				tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_);
			}
	} terminal_switcher;

	write_addr();
	for (;;) {
		int ch { get() };
		if (ch == EOF || ch == 0x04) { break; }
		if (ch >= '0' && ch <= '9') {
			add(ch, ch - '0');
		} else if (ch >= 'a' && ch <= 'f') {
			add(ch, ch - 'a' + 10);
		} else if (ch == '\r' || ch == '\n') {
			if (nibbles == 6) {
				if (valid_addr() && value <= 255) {
					buffer[addr] = static_cast<unsigned char>(value);
					put('\n');
				}
				clear();
			} else if (nibbles == 4) {
				if (valid_addr()) {
					value = buffer[addr];
					write_nibble(value >> 4);
					write_nibble(value & 0xf);
					put('\n');
				}
				clear();
			}
		}
	}
}
