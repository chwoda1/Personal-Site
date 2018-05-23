
build: 
	emcc -o main.html ./src/game.cpp -o3 -s WASM=1 -s ASSERTIONS=1 -s NO_EXIT_RUNTIME=1 --js-library library.js --shell-file build/index.html
	open main.html
clean: 
	rm main.js
	rm main.wasm 
	rm main.html
