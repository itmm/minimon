#include <iostream>
#include <termios.h>
#include <unistd.h>

unsigned long addr;
unsigned char value;
unsigned nibbles;

constexpr auto ADDR_NIBBLES { 2 * sizeof(addr) };

inline int get() { return std::cin.get(); }
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

void write_addr(unsigned long addr, int cnt) {
	if (cnt) { write_addr(addr >> 4, cnt - 1); }
	write_nibble(static_cast<int>(addr & 0xf));
}

inline void write_addr() {
	write_addr(addr, ADDR_NIBBLES - 1);
	put(": ");
}

void clear() {
	++addr;
	value = 0;
	nibbles = ADDR_NIBBLES;
	write_addr();
}


void add(char ch, int nibble) {
	if (nibbles == ADDR_NIBBLES + 2) {
		addr = value;
		nibbles = 2;
		put("\x1b[0E\x1b[2K");
		write_nibble(value >> 4);
		write_nibble(value & 0xf);
		value = 0;
	}
	if (nibbles < ADDR_NIBBLES) {
		addr = (addr << 4) + nibble;
	} else {
		value = (value << 4 ) + nibble;
	}
	++nibbles;
	put(ch);
	if (nibbles == ADDR_NIBBLES) { put(": "); }
}

void hello() {
	put("Hello World.\n");
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

	addr = reinterpret_cast<unsigned long>(hello);
	--addr;
	clear();
	for (;;) {
		int ch { get() };
		if (ch == EOF || ch == 0x04) { break; }
		if (ch >= '0' && ch <= '9') {
			add(ch, ch - '0');
		} else if (ch >= 'a' && ch <= 'f') {
			add(ch, ch - 'a' + 10);
		} else if (ch == '\r' || ch == '\n') {
			if (nibbles == ADDR_NIBBLES + 2) {
				*reinterpret_cast<unsigned char *>(addr) = value;
			} else if (nibbles == ADDR_NIBBLES) {
				value = *reinterpret_cast<unsigned char *>(addr);
				write_nibble(value >> 4);
				write_nibble(value & 0xf);
			} else {
				put('\a');
				continue;
			}
			put('\n');
			clear();
		} else if (ch == 'j' && nibbles == ADDR_NIBBLES) {
			put("jump\n");
			reinterpret_cast<void (*)()>(addr)();
			clear();
		} else {
			put('\a');
		}
	}
}
