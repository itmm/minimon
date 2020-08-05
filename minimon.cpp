
#line 4 "index.md"

	
#line 23 "index.md"

	#include <iostream>

#line 270 "index.md"

	#include <termios.h>
	#include <unistd.h>

#line 5 "index.md"

	
#line 29 "index.md"

	inline int get() {
		return std::cin.get();
	}
	inline void put(int ch) {
		std::cout.put(ch);
	}

#line 40 "index.md"

	void put(const char *str) {
		while (*str) { put(*str++); }
	}

#line 65 "index.md"

	
#line 84 "index.md"

	unsigned long addr;
	unsigned char value;
	unsigned nibbles;

#line 92 "index.md"

	constexpr auto ADDR_NIBBLES { 2 * sizeof(addr) };

#line 127 "index.md"

	void write_nibble(int nibble) {
		if (nibble >= 0) {
			if (nibble <= 9) {
				put('0' + nibble);
			} else if (nibble <= 15) {
				put('a' + nibble - 10);
			}
		}
	}

#line 174 "index.md"

	inline void write_byte(int byte) {
		write_nibble(byte >> 4);
		write_nibble(byte & 0xf);
	}

#line 66 "index.md"

	void add(char ch, int nibble) {
		
#line 183 "index.md"

	if (nibbles == ADDR_NIBBLES + 2) {
		addr = value;
		nibbles = 2;
		put("\x1b[0E\x1b[2K");
		write_byte(value);
		value = 0;
	}

#line 195 "index.md"

	if (nibbles < ADDR_NIBBLES) {
		addr = (addr << 4) + nibble;
	} else {
		value = (value << 4 ) + nibble;
	}
	++nibbles;
	put(ch);

#line 207 "index.md"

	if (nibbles == ADDR_NIBBLES) {
		
#line 168 "index.md"

	put(": ");

#line 209 "index.md"

	}

#line 68 "index.md"

	}

#line 98 "index.md"

	
#line 141 "index.md"

	void write_addr(
		unsigned long addr, int cnt
	) {
		if (cnt) {
			write_addr(
				addr >> 4, cnt - 1
			);
		}
		write_nibble(static_cast<int>(
			addr & 0xf
		));
	}

#line 99 "index.md"

	void clear() {
		
#line 158 "index.md"

	++addr;
	value = 0;
	nibbles = ADDR_NIBBLES;
	write_addr(addr, ADDR_NIBBLES - 1);
	
#line 168 "index.md"

	put(": ");

#line 163 "index.md"


#line 101 "index.md"

	}

#line 107 "index.md"

	void hello() {
		put("Hello World.\n");
	}

#line 6 "index.md"

	int main() {
		
#line 14 "index.md"

	
#line 115 "index.md"

	
#line 277 "index.md"

	class Terminal_Switcher {
			termios orig_;
		public:
			Terminal_Switcher() {
				
#line 292 "index.md"

	tcgetattr(STDIN_FILENO, &orig_);
	termios raw { orig_ };
	raw.c_lflag &= ~ (ECHO | ICANON);
	tcsetattr(
		STDIN_FILENO, TCSAFLUSH, &raw
	);

#line 282 "index.md"

			}
			~Terminal_Switcher() {
				
#line 303 "index.md"

	tcsetattr(
		STDIN_FILENO, TCSAFLUSH, &orig_
	);

#line 285 "index.md"

			}
	} terminal_switcher;

#line 116 "index.md"

	addr =
		reinterpret_cast<unsigned long>(
			hello
		);
	--addr;
	clear();

#line 15 "index.md"

	for (;;) {
		
#line 48 "index.md"

	int ch { get() };
	if (
		ch == EOF || ch == 0x04 ||
		ch == 'q'
	) {
		put("quit\n");
		break;
	}
	
#line 74 "index.md"

	else if (ch >= '0' && ch <= '9') {
		add(ch, ch - '0');
	} else if (ch >= 'a' && ch <= 'f') {
		add(ch, ch - 'a' + 10);
	}

#line 216 "index.md"

	else if (ch == '\r' || ch == '\n') {
		
#line 226 "index.md"

	if (nibbles == ADDR_NIBBLES + 2) {
		
#line 239 "index.md"

	*reinterpret_cast<unsigned char *>(
		addr
	) = value;

#line 228 "index.md"

	} else if (nibbles == ADDR_NIBBLES) {
		
#line 247 "index.md"

	value = *reinterpret_cast<
		unsigned char *
	>(addr);
	write_byte(value);

#line 230 "index.md"

	} else {
		put('\a');
		continue;
	}

#line 218 "index.md"

		put('\n');
		clear();
	}

#line 256 "index.md"

	else if (ch == 'j' &&
		nibbles == ADDR_NIBBLES
	) {
		put("jump\n");
		reinterpret_cast<
			void (*)()
		>(addr)();
		clear();
	}

#line 57 "index.md"

	else {
		put('\a');
	}

#line 17 "index.md"

	}

#line 8 "index.md"

	}
