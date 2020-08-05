#include <iostream>

int addr { 0 };
int value { 0 };
int nibbles { 0 };

void add(int nibble) {
	if (nibbles < 4) {
		addr = (addr << 4) + nibble;
	} else {
		value = (value << 4 ) + nibble;
	}
	++nibbles;
}

void write_nibble(int nibble) {
	if (nibble >= 0) {
		if (nibble <= 9) {
			std::cout.put('0' + nibble);
		} else if (nibble <= 15) {
			std::cout.put('a' + nibble - 10);
		}
	}
}

void clear() {
	addr = value = nibbles = 0;
}

int main() {
	unsigned char buffer[1000];
	for (;;) {
		int ch { std::cin.get() };
		if (ch == EOF) { break; }
		if (ch >= '0' && ch <= '9') {
			add(ch - '0');
		} else if (ch >= 'a' && ch <= 'f') {
			add(ch - 'a' + 10);
		} else if (ch == 'w') {
			if (addr >= 0 && addr < sizeof(buffer) && value >= 0 && value <= 255) {
				buffer[addr] = (unsigned char) value;
			}
			clear();
		} else if (ch == 'r') {
			if (addr >= 0 && addr < sizeof(buffer)) {
				value = buffer[addr];
				write_nibble(value >> 4);
				write_nibble(value & 0xf);
				std::cout.put('\n');
			}
			clear();
		}
	}
}
