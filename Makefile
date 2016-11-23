# For building the delux binary
all: 
	gcc -O3 delux.c -o delux
	chmod 755 delux
	@echo "Compilation done. Run \`make install\` to install delux"
install:
	@echo "Run this *after* you run \`make\` to compile the binary"
	@echo "Note: this operation requires root privileges"
	mkdir -p /etc/delux
	cp delux delux.legacy /usr/sbin/
	cp luxtab.csv /etc/delux/
	cp init.d/deluxd /etc/init.d/
	chown -R root: /etc/delux
	chown root: /etc/init.d/deluxd
	chown root: /usr/sbin/delux /usr/sbin/delux.legacy
clean:
	rm -f delux
