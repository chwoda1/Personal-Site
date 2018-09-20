
build: 
	emcc -o main.html ./src/game.cpp -o3 -s WASM=1 -s ASSERTIONS=1 -s NO_EXIT_RUNTIME=1 --js-library library.js --shell-file build/index.html
	open main.html
clean: 
	rm -f main.js
	rm -f main.wasm 
	rm -f main.html

deploy:
	emcc -o main.html ./src/game.cpp -o3 -s WASM=1 -s ASSERTIONS=1 -s NO_EXIT_RUNTIME=1 --js-library library.js --shell-file build/index.html
	cp main.html ~/Desktop/Release-Site
	cp main.js ~/Desktop/Release-Site
	cp main.wasm ~/Desktop/Release-Site
	cp -r static ~/Desktop/Release-Site/
	cp other.js ~/Desktop/Release-Site
	mv ~/Desktop/Release-Site/main.html ~/Desktop/Release-Site/index.html

