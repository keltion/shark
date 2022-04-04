CC = g++

all : shark

html_tokenizer.o : html_token.h html_tokenizer.h html_tokenizer.cc
	$(CC) -c html_tokenizer.cc

html_parser.o : html_token.h html_tokenizer.h html_parser.h html_parser.cc
	$(CC) -c html_parser.cc

main.o : main.cc html_parser.h
	$(CC) -c main.cc

shark : html_tokenizer.o html_parser.o main.o
	$(CC) -o shark main.o html_parser.o html_tokenizer.o
