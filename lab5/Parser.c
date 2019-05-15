#include<stdio.h>
char ch;
char get_char()
{
	return getchar();
}
void error()
{
	printf("该字符串是不合法的单词 \n");
	exit(0);
}
void Parser_A()
{
	switch(ch)
	{
	case 'a':
		if(ch=='a') ch=get_char(); else error();
		break;
	default: error();
	}
}
void Parser_B()
{
	switch(ch)
	{
	case 'b':
		break;
	case 'c':
		if(ch=='c') ch=get_char(); else error();
		break;
	default: error();
	}
}
void Parser_S()
{
	switch(ch)
	{
	case 'a':
		Parser_A();
		if(ch=='a') ch=get_char(); else error();
		Parser_S();
		break;
	case 'b':
		Parser_B();
		if(ch=='b') ch=get_char(); else error();
		Parser_S();
		break;
	case 'c':
		Parser_B();
		if(ch=='b') ch=get_char(); else error();
		Parser_S();
		break;
	case 'd':
		if(ch=='d') ch=get_char(); else error();
		break;
	default: error();
	}
}
int main()
{
	ch=get_char();
	Parser_S();
	if(ch!='#') error();
	printf("该字符串是合法的单词 \n");
	return 0;
}
