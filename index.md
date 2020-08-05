# Minimon - a minimal monitor program

```
@Def(file: minimon.cpp)
	@put(includes)
	@put(needed by main)
	int main() {
		@put(main)
	}
@End(file: minimon.cpp)
```

```
@def(main)
	@put(setup)
	for (;;) {
		@put(main loop)
	}
@end(main)
```

```
@def(includes)
	#include <iostream>
@end(includes)
```

```
@def(needed by main)
	inline int get() {
		return std::cin.get();
	}
	inline void put(int ch) {
		std::cout.put(ch);
	}
@end(needed by main)
```

```
@add(needed by main)
	void put(const char *str) {
		while (*str) { put(*str++); }
	}
@end(needed by main)
```

```
@def(main loop)
	int ch { get() };
	if (
		ch == EOF || ch == 0x04 ||
		ch == 'q'
	) {
		put("quit\n");
		break;
	}
	@put(other input cases)
	else {
		put('\a');
	}
@end(main loop)
```

```
@add(needed by main)
	@put(needed by add)
	void add(char ch, int nibble) {
		@put(add)
	}
@end(needed by main)
```

```
@def(other input cases)
	else if (ch >= '0' && ch <= '9') {
		add(ch, ch - '0');
	} else if (ch >= 'a' && ch <= 'f') {
		add(ch, ch - 'a' + 10);
	}
@end(other input cases)
```

```
@def(needed by add)
	unsigned long addr;
	unsigned char value;
	unsigned nibbles;
@end(needed by add)
```

```
@add(needed by add)
	constexpr auto ADDR_NIBBLES { 2 * sizeof(addr) };
@end(needed by add)
```

```
@add(needed by main)
	@put(needed by clear)
	void clear() {
		@put(clear)
	}
@end(needed by main)
```

```
@add(needed by main)
	void hello() {
		put("Hello World.\n");
	}
@end(needed by main)
```

```
@def(setup)
	@put(terminal switcher)
	addr =
		reinterpret_cast<unsigned long>(
			hello
		);
	--addr;
	clear();
@end(setup)
```

```
@add(needed by add)
	void write_nibble(int nibble) {
		if (nibble >= 0) {
			if (nibble <= 9) {
				put('0' + nibble);
			} else if (nibble <= 15) {
				put('a' + nibble - 10);
			}
		}
	}
@end(needed by add)
```

```
@def(needed by clear)
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
@end(needed by clear)
```

```
@def(clear)
	++addr;
	value = 0;
	nibbles = ADDR_NIBBLES;
	write_addr(addr, ADDR_NIBBLES - 1);
	@mul(write separator)
@end(clear)
```

```
@def(write separator)
	put(": ");
@end(write separator)
```

```
@add(needed by add)
	inline void write_byte(int byte) {
		write_nibble(byte >> 4);
		write_nibble(byte & 0xf);
	}
@end(needed by add)
```

```
@def(add)
	if (nibbles == ADDR_NIBBLES + 2) {
		addr = value;
		nibbles = 2;
		put("\x1b[0E\x1b[2K");
		write_byte(value);
		value = 0;
	}
@end(add)
```

```
@add(add)
	if (nibbles < ADDR_NIBBLES) {
		addr = (addr << 4) + nibble;
	} else {
		value = (value << 4 ) + nibble;
	}
	++nibbles;
	put(ch);
@end(add)
```

```
@add(add)
	if (nibbles == ADDR_NIBBLES) {
		@mul(write separator)
	}
@end(add)
```


```
@add(other input cases)
	else if (ch == '\r' || ch == '\n') {
		@put(handle return)
		put('\n');
		clear();
	}
@end(other input cases)
```

```
@def(handle return)
	if (nibbles == ADDR_NIBBLES + 2) {
		@put(set byte)
	} else if (nibbles == ADDR_NIBBLES) {
		@put(get byte)
	} else {
		put('\a');
		continue;
	}
@end(handle return)
```

```
@def(set byte)
	*reinterpret_cast<unsigned char *>(
		addr
	) = value;
@end(set byte)
```

```
@def(get byte)
	value = *reinterpret_cast<
		unsigned char *
	>(addr);
	write_byte(value);
@end(get byte)
```

```
@add(other input cases)
	else if (ch == 'j' &&
		nibbles == ADDR_NIBBLES
	) {
		put("jump\n");
		reinterpret_cast<
			void (*)()
		>(addr)();
		clear();
	}
@end(other input cases)
```

```
@add(includes)
	#include <termios.h>
	#include <unistd.h>
@end(includes)
```

```
@def(terminal switcher)
	class Terminal_Switcher {
			termios orig_;
		public:
			Terminal_Switcher() {
				@put(switch to raw)
			}
			~Terminal_Switcher() {
				@put(switch back)
			}
	} terminal_switcher;
@end(terminal switcher)
```

```
@def(switch to raw)
	tcgetattr(STDIN_FILENO, &orig_);
	termios raw { orig_ };
	raw.c_lflag &= ~ (ECHO | ICANON);
	tcsetattr(
		STDIN_FILENO, TCSAFLUSH, &raw
	);
@end(switch to raw)
```

```
@def(switch back)
	tcsetattr(
		STDIN_FILENO, TCSAFLUSH, &orig_
	);
@end(switch back)
```

