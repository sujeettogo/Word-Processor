#include<graphics.h>
#include<string.h>
#include<fstream.h>
#include<conio.h>
#include<stdlib.h>
#include<ctype.h>
#include<dirent.h>

#define true 1
#define false 0

#define up 72
#define down 80
#define left 75
#define right 77
#define esc 27
#define enter 13
#define bks 8

REGS in_regs, out_regs;

int active = 0, activetot = 1;
int x, y, cl;
char filename[5][100];
char ftxt[5][100];
char faux[5][100];
int heap_mem_cnt=0;
int b_mode=false;
init_mouse()
{
	in_regs.x.ax = 0;
	int86(0x33, &in_regs, &out_regs);
	return out_regs.x.ax;
}
void show_mouse() {
	in_regs.x.ax = 1;
	int86(51, &in_regs, &out_regs);
}
int mouse_press() {
	in_regs.x.ax = 3;
	int86(0x33, &in_regs, &out_regs);
	return(out_regs.x.bx);
}
void initialize_graphics()
{
	int gdriver = DETECT, gmode;
	initgraph(&gdriver, &gmode, "C:\\BGI\\");
}
void mouse_pos(int& xc, int& yc, int& cl)
{
	in_regs.x.ax = 3;
	int86(0x33, &in_regs, &out_regs);
cl = out_regs.x.bx;
	xc = out_regs.x.cx;
	yc = out_regs.x.dx;
}
void close()
{
	mouse_pos(x, y, cl);
	if (x >= getmaxx() - 18 && x <= getmaxx() && y >= 0 && y <= 20 && cl == 1)
		exit(2);
}
struct password_data
{
	char filename[100],password[100];
};
void input(char *req)
{
	    int i = 0;
		char c;
		while (1 && c != enter)
		{
			close();
			c = getch();
			if (isalpha(c))	
			{
				outtext("*");
				req[i] = c;
				++i;
			}	
		}
		req[i]='\0';
}
void password_regis(char *name)
{
	setfillstyle(SOLID_FILL, BLUE);
	settextstyle(0,0,1);	
	bar(30, 80, 600, 200);
	setcolor(WHITE);
	outtextxy(40,140,"Enter the password:");
	moveto(250,140);
	fstream f;
	password_data p;
	f.open("regis.dll",ios::app|ios::out|ios::binary);
	strcpy(p.filename,name);
	input(p.password);
	f.write((char*)&p,sizeof(p));
	//cout<<"Pass word set :"<<p.password;
	f.close();
}
int verify_password(char *password)
{
	char mem[100];
	outtextxy(40,140,"Enter the password:");
	moveto(250,140);
	input(mem);
	if(strcmpi(mem,password)==0)
	{
		return true;
	}
	else
		return false;
}
int is_password(char *name,char *password)
{
	fstream f;
	password_data p;
	f.open("regis.dll",ios::app|ios::in|ios::binary);
	while(f.read((char*)&p,sizeof(p)))
	{
		if(strcmpi(name,p.filename)==0)
		{
			setfillstyle(SOLID_FILL, BLUE);
	        settextstyle(0,0,1);	
	        bar(30, 80, 600, 200);
	        setcolor(WHITE);
			outtextxy(40,100,"This file is password protected");
			strcpy(password,p.password);
			return true;
		}
	}
	return false;
}
void search_mode(char *name,char find[50])
{
	
	char mem[100],s1[50],s2[50];
	closegraph();
	int y=3;
	cprintf("SEARCH MODE");
	gotoxy(5,3);
	fstream f;
	f.open(name,ios::in);
	int j=0,done;
	textcolor(WHITE);
	
	while(!f.eof())
	{
		done=false;
		f.getline(mem,100,' ');
		for(int i=0;i<strlen(mem);++i)
		{
			if(mem[i]=='\n')
			{
				for( j=0;j<i;++j)
				{
					s1[j]=mem[j];
				}
				s1[j]='\0';
				
				if(strcmpi(s1,find)==0)
				{
					textcolor(GREEN);
				}
			cprintf("%s ",s1);
			textcolor(WHITE);
				y++;
				gotoxy(5,y);
				int k=0;
				for( j=i+1;j<strlen(mem);++j)
				{
					s2[k]=mem[j];
					k++;
				}
				s2[k]='\0';
				if(strcmpi(s2,find)==0)
				{
					textcolor(GREEN);
				}
				cprintf("%s ",s2);
				textcolor(WHITE);
				done=true;
				break;
			}
		}
		if(done==false)
		{
			if(strcmpi(mem,find)==0)
				textcolor(GREEN);
			cprintf("%s ",mem);
			textcolor(WHITE);
		}
	}
	f.close();
	getch();
	show_mouse();
	initialize_graphics();
}
void what_to_search(char *name)
{
	char find[50];
	setfillstyle(SOLID_FILL, BLUE);
	settextstyle(0,0,1);	
	bar(30, 80, 600, 200);
	setcolor(WHITE);
	outtextxy(40,140,"Enter the string you want:");
	moveto(250,140);
	int i = 0;
		char c;
		while (1 && c != enter)
		{
			close();
			c = getch();
			char k[2];
			k[0] = c;
			k[1] = '\0';
			if (c != enter)
				outtext(k);
			if (c != enter )
			{
				find[i] = c;
				++i;
			}	
		}
		find[i]='\0';
		search_mode(name,find);
}
void draw_bullet(int x,int y)
{
	circle(x,y,2);
}
class loading
{
public:
	void load()
	{
		int x=170,i;
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		outtextxy(170,60,"WELCOME TO HEX EDITOR");
		outtextxy(170,180," LOADING,PLEASE WAIT");
		settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
		outtextxy(100,390," click help menu with placing you mouse over 'HELP' and press ");
		outtextxy(100,400,"tilde(~) but without shift-that is '`' after opening a dummy file");
		outtextxy(200,430,"This software uses an autosave feature");
		for(i=0;i<300;++i)
		{
			delay(30);
			line(x,200,x,220);
			x++;
		}

	}
};
class window_elements
{
public:
	void draw_window()//window
	{
		setfillstyle(1, 7);//7
		bar(0, 0, getmaxx(), getmaxy());
	}

	void menu_bar()//blue bar
	{
		setfillstyle(1, 1); //colour of menu bar
		bar(0, 0, getmaxx(), 20);
	}

	void close_button()//close button
	{
		setfillstyle(1, 4);
		bar(getmaxx() - 18, 0, getmaxx(), 20);
	}
	void X_mark()
	{
		outtextxy(getmaxx() - 10, 7, "X");
	}
	void white_bar()
	{
		setfillstyle(1, 15);
		bar(0, 20, getmaxx(), 40);
	}
	void final_window()//for drawing all the contents
	{
		draw_window();
		menu_bar();
		close_button();
		X_mark();
		white_bar();
	}
};
void textstyle_box(int font, int char_size)
{
	bar(220, 20, 240, 39);
	line(220, 20, 220, 39);
	line(240, 39, 240, 20);
	outtextxy(228, 26, "T");
}

char *fontnames[] = { "DEFAULT font",  "TRIPLEX font",

"SMALL font",    "SANS SERIF_font",

"GOTHIC_font",   "SCRIPT font",

"SIMPLEX font",  "TRIPLEX SCRIPT font",

"COMPLEX font",  "EUROPEAN font",

"BOLD font" };
int textstyle_box_after_click(int color)
{
	setfillstyle(SOLID_FILL, BLUE);
	bar(30, 80, 300, 400);
	char c[2];
	c[0] = '0';
	c[1] = '\0';
	setcolor(WHITE);
	settextstyle(0, 0, 1);
	int j = 0;
	for (int i = DEFAULT_FONT; i <= BOLD_FONT; i++)
	{
		outtextxy(50, 90 + j, c);
		c[0]++;
		outtextxy(100, 90 + j, fontnames[i]);
		j = j + 10;
	}
	outtextxy(50, 220, "Enter your choice below:");
	setfillstyle(SOLID_FILL, WHITE);
	bar(50, 230, 100, 240);
	char ch;
	setcolor(BLACK);
	moveto(75, 235);
	while (ch != '\n')
	{
		ch = getch();
		c[0] = ch;
		c[1] = '\0';
		if (ch != '\n')
			outtext(c);
		if (ch == '0')
			return 0;
		if (ch == '1')
			return 1;
		if (ch == '2')
			return 2;
		if (ch == '3')
			return 3;
		if (ch == '4')
			return 4;
		if (ch == '5')
			return 5;
		if (ch == '6')
			return 6;
		if (ch == '7')
			return 7;
		if (ch == '8')
			return 8;
		if (ch == '9')
			return 9;
		if (ch == ':')
			return 10;
		if (ch == '-')
			return -1;
	}
	setcolor(color);
}
void draw_shape()
{
	line(270, 20, 270, 39);
	line(290, 39, 290, 20);
	rectangle(275, 25, 285, 35);
}

char *shapes[] = { "Rectangle","Arc","Circle","Line" };
int draw_shape_click(int color)
{
	setfillstyle(SOLID_FILL, BLUE);
	bar(30, 80, 300, 170);
	setcolor(WHITE);
	char c[2];
	c[0] = '0';
	c[1] = '\0';
	settextstyle(0, 0, 1);
	int j = 0;
	for (int i = 0; i<4; i++)
	{
		outtextxy(50, 90 + j, c);
		c[0]++;
		outtextxy(100, 90 + j, shapes[i]);
		j = j + 10;
	}
	outtextxy(50, 140, "Enter your choice below:");
	setfillstyle(SOLID_FILL, WHITE);
	bar(50, 150, 100, 160);
	char ch;
	setcolor(BLACK);
	moveto(75, 155);
	while (ch != '\n')
	{
		ch = getch();
		c[0] = ch;
		c[1] = '\0';
		if (ch != '\n')
			outtext(c);
		if (ch == '0')
			return 0;
		if (ch == '1')
			return 1;
		if (ch == '2')
			return 2;
		if (ch == '3')
			return 3;
		if (ch == '-')
			return -1;
	}
	setcolor(color);
}
struct rectangle_data
{
	int ix, iy, fx, fy;
};
struct arc_data
{
	int cx, cy, ia, fa, r;
};
struct circle_data
{
	int cx, cy, r;
};
struct line_data
{
	int ix, iy, fx, fy;
};
struct dat
{
	rectangle_data r;
	arc_data a;
	circle_data c;
	line_data l;
};
struct colourdat
{
	int linex, liney, color, font;
	int shape_rect, shape_circle, shape_arc, shape_data;
	dat shape;
};

void if_rectangle(int color, int textstyle, int linex, int liney, colourdat &c, fstream &k1)
{
	//use after cleardevice and drawing all elements
	char c1[2];
	char number[5];
	int num, j = 0;
	c.shape_rect = true;
	c.shape_circle = false;
	c.shape_arc = false;
	c.shape_data = false;

	setfillstyle(SOLID_FILL, BLUE);
	bar(60, 60, 450, 300);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE);
	setcolor(WHITE);
	outtextxy(80, 80, "Enter the x-coordinate of starting position:");
	bar(80, 90, 100, 100);
	setcolor(BLACK);
	moveto(90, 95);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.r.ix = num;

	setcolor(WHITE);
	c1[0] = 0;
	outtextxy(80, 110, "Enter the y-coordinate of starting position:");
	bar(80, 120, 100, 130);
	setcolor(BLACK);
	moveto(90, 125);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.r.iy = num;
	c1[0] = 0;

	setcolor(WHITE);
	outtextxy(80, 140, "Enter the x-coordinate of final position:");
	bar(80, 150, 100, 160);
	setcolor(BLACK);
	moveto(90, 155);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);

	c.shape.r.fx = num;
	c1[0] = 0;
	setcolor(WHITE);
	outtextxy(80, 170, "Enter the y-coordinate of final position:");
	bar(80, 180, 100, 190);
	setcolor(BLACK);
	moveto(90, 185);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);

	c.shape.r.fy = num;
	//rectangle(c.shape.r.ix,c.shape.r.iy,c.shape.r.fx,c.shape.r.fy);
	c.linex = linex;
	c.liney = liney;
	c.color = color;
	c.font = textstyle;
	settextstyle(textstyle, 0, 0);
	k1.open(faux[active], ios::out | ios::in | ios::binary | ios::app);
	k1.write((char*)&c, sizeof(c));
	k1.close();
	setcolor(color);

}
void if_arc(int color, int textstyle, int linex, int liney, colourdat &c, fstream &k1)
{
	c.shape_rect = false;
	c.shape_circle = false;
	c.shape_arc = true;
	c.shape_data = false;
	char c1[2];
	char number[5];
	int num, j = 0;
	setfillstyle(SOLID_FILL, BLUE);
	bar(60, 60, 450, 300);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE);
	setcolor(WHITE);
	outtextxy(80, 80, "Enter the x-coordinate of centre:");
	bar(80, 90, 100, 100);
	setcolor(BLACK);
	moveto(90, 95);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.a.cx = num;

	setcolor(WHITE);
	c1[0] = 0;
	outtextxy(80, 110, "Enter the y-coordinate of centre:");
	bar(80, 120, 100, 130);
	setcolor(BLACK);
	moveto(90, 125);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.a.cy = num;
	c1[0] = 0;

	outtextxy(80, 140, "Enter the radius:");
	bar(80, 150, 100, 160);
	setcolor(BLACK);
	moveto(90, 155);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.a.r = num;

	c1[0] = 0;
	setcolor(WHITE);
	outtextxy(80, 170, "Enter the starting angle:");
	bar(80, 180, 100, 190);
	setcolor(BLACK);
	moveto(90, 185);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.a.ia = num;
	c1[0] = 0;
	setcolor(WHITE);
	outtextxy(80, 200, "Enter the ending angle:");
	bar(80, 210, 100, 220);
	setcolor(BLACK);
	moveto(90, 215);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);

	c.shape.a.fa = num;
	c.linex = linex;
	c.liney = liney;
	c.color = color;
	c.font = textstyle;
	settextstyle(textstyle, 0, 0);
	k1.open(faux[active], ios::out | ios::in | ios::binary | ios::app);
	k1.write((char*)&c, sizeof(c));
	k1.close();
	setcolor(color);

}
void if_circle(int color, int textstyle, int linex, int liney, colourdat &c, fstream &k1)
{
	c.shape_rect = false;
	c.shape_circle = true;
	c.shape_arc = false;
	c.shape_data = false;
	char c1[2];
	char number[5];
	int num, j = 0;
	setfillstyle(SOLID_FILL, BLUE);
	bar(60, 60, 450, 300);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE); 
	setcolor(WHITE);
	outtextxy(80, 80, "Enter the x-coordinate of centre:");
	bar(80, 90, 100, 100);
	setcolor(BLACK);
	moveto(90, 95);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.c.cx = num;

	setcolor(WHITE);
	c1[0] = 0;
	outtextxy(80, 110, "Enter the y-coordinate of centre:");
	bar(80, 120, 100, 130);
	setcolor(BLACK);
	moveto(90, 125);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.c.cy = num;
	c1[0] = 0;

	outtextxy(80, 140, "Enter the radius:");
	bar(80, 150, 100, 160);
	setcolor(BLACK);
	moveto(90, 155);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.c.r = num;
	c.linex = linex;
	c.liney = liney;
	c.color = color;
	c.font = textstyle;
	settextstyle(textstyle, 0, 0);
	k1.open(faux[active], ios::out | ios::in | ios::binary | ios::app);
	k1.write((char*)&c, sizeof(c));
	k1.close();
	setcolor(color);

}
void if_line(int color, int textstyle, int linex, int liney, colourdat &c, fstream &k1)
{
	//use after cleardevice and drawing all elements
	char c1[2];
	char number[5];
	int num, j = 0;
	c.shape_rect = false;
	c.shape_circle = false;
	c.shape_arc = false;
	c.shape_data = true;

	setfillstyle(SOLID_FILL, BLUE);
	bar(60, 60, 450, 300);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, WHITE);
	setcolor(WHITE);
	outtextxy(80, 80, "Enter the x-coordinate of starting position:");
	bar(80, 90, 100, 100);
	setcolor(BLACK);
	moveto(90, 95);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.l.ix = num;

	setcolor(WHITE);
	c1[0] = 0;
	outtextxy(80, 110, "Enter the y-coordinate of starting position:");
	bar(80, 120, 100, 130);
	setcolor(BLACK);
	moveto(90, 125);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);
	c.shape.l.iy = num;
	c1[0] = 0;

	setcolor(WHITE);
	outtextxy(80, 140, "Enter the x-coordinate of final position:");
	bar(80, 150, 100, 160);
	setcolor(BLACK);
	moveto(90, 155);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);

	c.shape.l.fx = num;
	c1[0] = 0;
	setcolor(WHITE);
	outtextxy(80, 170, "Enter the y-coordinate of final position:");
	bar(80, 180, 100, 190);
	setcolor(BLACK);
	moveto(90, 185);
	while (c1[0] != enter)
	{
		c1[0] = getch();
		c1[1] = '\0';
		number[j] = c1[0];
		j++;
		if (c1[0] != enter)
			outtext(c1);
	}
	number[j] = '\0';
	j = 0;
	num = atoi(number);

	c.shape.l.fy = num;
	//rectangle(c.shape.r.ix,c.shape.r.iy,c.shape.r.fx,c.shape.r.fy);
	c.linex = linex;
	c.liney = liney;
	c.color = color;
	c.font = textstyle;
	settextstyle(textstyle, 0, 0);
	k1.open(faux[active], ios::out | ios::in | ios::binary | ios::app);
	k1.write((char*)&c, sizeof(c));
	k1.close();
	setcolor(color);

}
void colourbox(int COLOR)
{
	setfillstyle(SOLID_FILL, COLOR);
	bar(160, 25, 200, 35);
	setcolor(BLACK);
	line(160, 25, 200, 25);
	line(160, 25, 160, 35);
	line(200, 25, 200, 35);
	line(160, 35, 200, 35);
	setcolor(COLOR);
}
void draw_click_color_box()
{
	setfillstyle(SOLID_FILL, WHITE);
	bar(160, 45, 210, 135);
	setfillstyle(SOLID_FILL, 0);
	bar(160, 45, 210, 60);
	setfillstyle(SOLID_FILL, 1);
	bar(160, 60, 210, 75);
	setfillstyle(SOLID_FILL, 2);
	bar(160, 75, 210, 90);
	setfillstyle(SOLID_FILL, 4);
	bar(160, 90, 210, 105);
	setfillstyle(SOLID_FILL, YELLOW);
	bar(160, 105, 210, 120);
	setfillstyle(SOLID_FILL, WHITE);
	bar(160, 120, 210, 135);
}
void windows_sel()
{
	setfillstyle(SOLID_FILL, BLUE);
	bar(0, 40, getmaxx(), 60);
	setfillstyle(SOLID_FILL, WHITE);
	int j = 0;
	for (int i = 0; i<activetot; ++i)
	{
		bar(0 + j, 45, 140 + j, 60);
		outtextxy(10 + j, 50, ftxt[i]);
		j = j + 150;
	}
}

class menu_elements
{
public:
	void menu_button()
	{
		setcolor(BLACK);
		outtextxy(20, 27, "MENU");
	}
	void edit_button()
	{
		outtextxy(90, 27, "EDIT");
		outtextxy(600,27,"HELP");
	}

	void final_menu_opt()
	{
		menu_button();
		edit_button();
		draw_shape();
		windows_sel();
		textstyle_box(0, 0);
	}
};
class workspace
{
public:
	void white_space()
	{
		setfillstyle(1, 15);
		bar(0, 60, getmaxx(), getmaxy());
	}
};

void temp()
{
	settextstyle(0, 0, 1);
	setfillstyle(SOLID_FILL, BLUE);
	bar(20, 45, 130, 150);
	setcolor(WHITE);
	outtextxy(25, 50, "NEW FILE");
	line(20, 75, 130, 75);
	outtextxy(25, 85, "OPEN FILE");
	line(20, 115, 130, 115);
	outtextxy(25, 120, "CLOSE");
	line(20,135,130,135);
	outtextxy(25,140,"SET PASSWORD");
}
void cursor_loc(int x,int y,int textstyle,int color)
{
	char kx[5],ky[5];
	itoa(x,kx,10);
	itoa(y,ky,10);
	settextstyle(0,0,1);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL, BLUE);
	bar(getmaxx()-100,80,getmaxx(),100);
	outtextxy(getmaxx()-70,90,kx);
	outtextxy(getmaxx()-40,90,ky);
	settextstyle(textstyle, 0, 1);
	setcolor(color);
}

void rulerx()
{
	setfillstyle(SOLID_FILL,WHITE);
	char *num[]={"0","100","200","300","400","500","600"};
	bar(0,61,getmaxx(),80);
	for(int i=0;i<=getmaxx();++i)
	{
		if(i%100==0)
		{
			outtextxy(i,70,num[i/100]);
		}
	}
}
void rulery()
{
	bar(0,80,25,getmaxy());
	char *num[]={"0","100","200","300","400","500","600"};
	for(int i=80;i<=getmaxy();++i)
	{
		if(i%100==0)
		{
			outtextxy(0,i,num[i/100]);
		}
	}
}
void directory()
{
	setfillstyle(SOLID_FILL,BLUE);
	bar(25,150,getmaxx(),getmaxy());
	int posx=25,posy=150;
	
	DIR *pdir = NULL; 
    pdir = opendir ("."); 
    struct dirent *pent = NULL;
    if (pdir == NULL) 
    { 
        cout << "\nERROR! pdir could not be initialised correctly";
        exit (3);
    } 
    setcolor(WHITE);
    while (pent = readdir (pdir))
    {
        if (pent == NULL) 
        { 
            cout << "\nERROR! pent could not be initialised correctly";
            exit (3);
        }
        outtextxy( posx,posy,pent->d_name );
		posy=posy+10;
		if(posy>=getmaxy())
		{
			posx=posx+120;
			posy=150;
		}
    }


    closedir (pdir);
	setcolor(BLACK);
}

void ruler()
{
	rulerx();
	rulery();
}
void help_box()
{
	setfillstyle(SOLID_FILL,BLUE); 
	setcolor(WHITE);
	settextstyle(0,0,1);
	bar(0, 60, getmaxx(), getmaxy());
	outtextxy(200,85,"WELCOME TO THE HELP MENU ");
	outtextxy(20,120,"To access an option using a mouse point the mouse pointer to that location ");
	outtextxy(20,140,"and press the tilde button( ` )");
	outtextxy(20,160,"To toggle underline mode press (shift + tilde(`))");
	outtextxy(20,180,"Specific shapes can be drawn in the text file by using the option square in ");
	outtextxy(20,200,"the menu bar.The available shapes are rectangle,arc,circle and line ");
	outtextxy(20,220,"multitab facilities search tab and bullet points facilities are available");
	outtextxy(20,240,"Backspace and escape buttons should not be used while writing in the file name ");
	outtextxy(20,280,"Press escape to quit hex editor");
	outtextxy(20,300,"DONE BY");
	outtextxy(20,310,"G.RAJAGOPALAN");
	outtextxy(20,320,"A.SUJEET TOGO");
	outtextxy(20,330,"CLASS XII-A");
	outtextxy(20,340,"P.S.SENIOR SECONDARY SCHOOL");
	outtextxy(20,370,"Press any to close help menu");
	getch();
}
class openfile
{
private:
	window_elements w;
	menu_elements m;
	fstream f;
public:
	void draw_req_window_elements1()
	{
		settextstyle(0, 0, 1);
		w.final_window();
		m.final_menu_opt();
		ruler();
	}
	void name_of_openfile()
	{
		
		settextstyle(0, 0, 1);
		directory();
		outtextxy(50, 100, "Enter the name of the file<without extension>:");
		moveto(420, 100);
		int i = 0;
		char c;
		while (1 && c != enter)
		{
			close();
			c = getch();
			char k[2];
			k[0] = c;
			k[1] = '\0';
			if (c != enter)
				outtext(k);
			if (c != enter)
				filename[active][i] = c;
			++i;
		}
		filename[active][i] = '\0';

		strcpy(ftxt[active], filename[active]);
		strcat(ftxt[active], ".txt");
		strcpy(faux[active], filename[active]);
		strcat(faux[active], ".aux");

		draw_req_window_elements1();
		colourbox(getcolor());
		setcolor(WHITE);
		char str[] = "C:\\TURBOC++\\";
		strcat(str, ftxt[active]);
		outtextxy(35, 5, str);
		setcolor(0);
		f.open(ftxt[active], ios::out | ios::app/* |ios::nocreate */);
		f.close();
		int flag;
		char password[100];
		flag=is_password(ftxt[active],password);
		if(flag==true)
		{
			int accept;
			accept=verify_password(password);
			if(accept==1)
			{
				cleardevice();
				draw_req_window_elements1();
				colourbox(getcolor());
				setcolor(WHITE);
				outtextxy(35, 5, str);
				setcolor(0);
				after_open_file(ftxt[active]);
			}
			else
			{
				outtextxy(200,250,"Wrong password user access revoked");
				getch();
				cleardevice();
				strcpy(ftxt[active],"untitled.txt");
				draw_req_window_elements1();
				colourbox(getcolor());
				setcolor(WHITE);
				outtextxy(35, 5, str);
				setcolor(0);
				name_of_openfile();
			}
		}
		else
		after_open_file(ftxt[active]);

	}
	void edit(int& b_mode)
	{
		int run=true;
		setfillstyle(SOLID_FILL, BLUE);
		settextstyle(0, 0, 1);
		setfillstyle(SOLID_FILL, BLUE);
		bar(90, 45, 160, 100);
		setcolor(WHITE);
		outtextxy(100, 50, "SEARCH");
		line(90, 75, 160, 75);
		outtextxy(100, 85, "BULLETS");
		line(90, 100, 160, 100);
		char c;
	while(run==true)
		{
			mouse_pos(x,y,cl);
			c=getch();
			if(x>=90 && x<=160 && y>=45 && y<=75)
			{
				run=false;
				what_to_search(ftxt[active]);
			}
			if(x>=90 && x<=160 && y>75 && y<=100)
			{
				run=false;
				fstream f;
				f.open(ftxt[active],ios::out|ios::app);
				if(b_mode==true)
					b_mode=false;
				else
					b_mode=true;
				f.put('@');
				f.close();
			}
		}
		
	}
	void after_open_file(char filename[100])
	{
		heap_mem_cnt++;
		int b_mode=false;
		settextstyle(0, 0, 1);
		fstream k2;
		setcolor(BLACK);
		colourdat d;
		int underline_mode=false;
		if (!f)
		{
			cerr << "Create a new file";
			f.close();
			f.open(ftxt[active], ios::out);
			f.close();
			k2.open(faux[active], ios::binary | ios::out);
			k2.close();
		}
		k2.open(faux[active], ios::binary | ios::in);
		f.open(filename, ios::in);
		int linex = 30, liney = 100, run = true,spacex=10,spacey=10;
		char c1;
		k2.read((char*)&d, sizeof(d));
		fstream k1;
		int mode = false, modecolor = false, color1=0, mode_menu = false, mode_text = false;
		while (!f.eof())
		{
			moveto(linex, liney);
			c1 = f.get();
			char k[2];
			k[0] = c1;
			k[1] = '\0';
			if (d.linex == linex && d.liney == liney)
			{
				//cout<<linex<<' '<<liney<<' '<<d.linex<<' '<<d.liney<<' '<<color;
				color1 = d.color;
				setcolor(color1);
				settextstyle(d.font, 0, 1);
				switch(d.font)
				{
					case 0:
					spacex=10;
					spacey=10;
					break;
					case 1:
					spacex=12;
					spacey=15;
					break;
					case 2:
					spacex=6;
					spacey=10;
					break;
					case 3:
					spacex=10;
					spacey=13;
					break;
					case 4:
					spacex=10;
					spacey=13;
					break;
					case 5:
					spacex=10;
					spacey=13;
					break;
					case 6:
					spacex=10;
					spacey=13;
					break;
					case 7:
					spacex=10;
					spacey=13;
					break;
					case 8:
					spacex=12;
					spacey=15;
					break;
					case 9:
					spacex=15;
					spacey=24;
					break;
					case 10:
					spacex=15;
					spacey=24;
					break;
				}
				if (d.shape_rect == true)
				{
					rectangle(d.shape.r.ix, d.shape.r.iy, d.shape.r.fx, d.shape.r.fy);
				}
				if (d.shape_circle == true)
				{
					circle(d.shape.c.cx, d.shape.c.cy, d.shape.c.r);
				}
				if (d.shape_arc == true)
				{
					arc(d.shape.a.cx, d.shape.a.cx, d.shape.a.ia, d.shape.a.fa, d.shape.a.r);
				}
				if (d.shape_data == true)
				{
					line(d.shape.l.ix, d.shape.l.iy, d.shape.l.fx, d.shape.l.fy);
				}
				k2.read((char*)&d, sizeof(d));
			}

			switch (c1)
			{
			case '\n':
				liney = liney + spacey;
				linex = 30;
				if(b_mode==true)
				{
					draw_bullet(linex,liney);
					linex=40;
				}
				break;
				case '$':
				setfillstyle(1, 7);
				bar(linex-spacex,liney+spacey,linex,liney);
				linex=linex-spacex;
				break;
			case 32:
				linex = linex + spacex;
				break;
				case '~':
				if(underline_mode==false)
				{
					underline_mode=true;
				}
				else
					underline_mode=false;
				break;
				case '@':
				if(b_mode==false)
					b_mode=true;
				else
					b_mode=false;
				break;
			default:
				if (isalpha(c1))
				{
					linex = linex + spacex;
					outtext(k);
				}
				if(underline_mode==true)
					outtext("_");
			}
		}
		k2.close();
		f.close();
		k1.open(faux[active], ios::out | ios::binary | ios::app);
		k1.close();
		long pos;
		colourdat c;
		int color = color1, textstyle=d.font, shape;
		f.open(filename, ios::out | ios::app);
			//(20,115)->(130,130)
		while (1 && run == true)
		{
			char ch;
			//close();
			mouse_pos(x, y, cl);
			cursor_loc(linex,liney,textstyle,color);
			if (x >= getmaxx() - 18 && x <= getmaxx() && y >= 0 && y <= 20 && cl == 1)
			{
				run = false;
				f.close();
			}
			if (linex >= getmaxx())
			{
				linex = 30;
				liney = liney + spacey;
				f.put('\n');
			}
			if (x >= 20 && x <= 40 && y >= 20 && y <= 40 && mode_menu == false)
			{
				temp();
				mode_menu = true;
			}
			if(x >= 90 && x <= 110 && y >= 20 && y <= 40 && mode_menu == false)
			{
				f.close();
				edit(b_mode);
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
			}
			if(x>=600 && x<=650 && y>=20 && y<=40)
			{
				help_box();
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
			}
			if (x >= 270 && x <= 290 && y >= 20 && y <= 40 && mode_text == false)
			{
				mode_text = true;
				shape = draw_shape_click(color);
			}
			
			if(x>=20 && x<=130 && y>=115 && y<=130 && mode_menu==true)
			{
				mode_menu=false;
				for(int i=active;i<=activetot;++i)
				{
					strcpy(ftxt[i],ftxt[i+1]);
					strcpy(faux[i],faux[i+1]);
				}
				activetot--;
				if(active!=0)
				active--;
			else
				active=0;
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
			}
			
			
			if (shape == 0 && mode_text == true)
			{
				if_rectangle(color, textstyle, linex, liney, c, k1);
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
				mode_text = false;
			}
			if (shape == 1 && mode_text == true)
			{
				if_arc(color, textstyle, linex, liney, c, k1);
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				
				after_open_file(ftxt[active]);
				mode_text = false;
			}
			if (shape == 2 && mode_text == true)
			{
				if_circle(color, textstyle, linex, liney, c, k1);
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
				mode_text = false;
			}
			if (shape == 3 && mode_text == true)
			{
				if_line(color, textstyle, linex, liney, c, k1);
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
				mode_text = false;
			}
			if (x >= 220 && x <= 240 && y >= 20 && y <= 40 && mode_text == false)
			{
				textstyle = textstyle_box_after_click(color);
				c.linex = linex;
				c.liney = liney;
				c.color = color;
				c.font = textstyle;
				k1.open(faux[active], ios::out | ios::in | ios::binary | ios::app);
				k1.write((char*)&c, sizeof(c));
				k1.close();
				modecolor = false;
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
				mode_menu = false;
			}
			if(heap_mem_cnt==25)
			{
				cerr<<"Sorry hex editor either ran out of memory or some error has occured:";
				exit(2);
			}
			if (x >= 20 && x <= 130 && y >= 45 && y <= 75 && mode_menu == true)
			{
				mode_menu = false;
				activetot++;
				active = activetot - 1;
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				name_of_openfile();
			}
			
			if(x >= 25 && x <= 130 && y >= 135 && y <= 150 && mode_menu==true)
			{
				password_regis(filename);
				f.close();
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				run=false;
				after_open_file(ftxt[active]);
				mode_menu=false;
			}
			if (!(x >= 20 && x <= 130 && y >= 20 && y <= 130) && mode_menu == true)
			{
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(ftxt[active]);
				mode_menu = false;
			}
			
			if (x >= 0 && x <= 140 && y >= 45 && y <= 60 && mode_menu != true && mode != true && activetot>=0 && active!=0)
			{
				cleardevice();
				active = 0;
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);			
				after_open_file(ftxt[active]);
				delay(20);
			}
			if (x >= 150 && x <= 290 && y >= 45 && y <= 60 && mode_menu != true && mode != true && activetot>=1 && active!=1)
			{
				cleardevice();
				active = 1;
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);				
				after_open_file(ftxt[active]);
				delay(20);
			}
			if (x >= 300 && x <= 440 && y >= 45 && y <= 60 && mode_menu != true && mode != true && activetot>=2 && active!=2)
			{
				cleardevice();
				active = 2; 
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);				
				after_open_file(ftxt[active]);
				delay(20);
			}
			if (x >= 450 && x <= 590 && y >= 45 && y <= 60 && mode_menu != true && mode != true && activetot>=3 && active!=3)
			{
				cleardevice();
				active = 3;
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);				
				after_open_file(ftxt[active]);
				delay(20);
			}
			
			if (x >= 160 && x <= 200 && y >= 25 && y <= 35)
			{
				draw_click_color_box();
				mode = true;
			}

			if (x >= 160 && x <= 210 && y >= 45 && y <= 60 && mode == true)
			{
				setcolor(0);
				color = 0;
				modecolor = true;
			}

			if (x >= 160 && x <= 210 && y >= 60 && y <= 75 && mode == true)
			{
				setcolor(1);
				color = 1;
				modecolor = true;
			}
			if (x >= 160 && x <= 210 && y >= 75 && y <= 90 && mode == true)
			{
				setcolor(2);
				color = 2;
				modecolor = true;
			}
			if (x >= 160 && x <= 210 && y >= 90 && y <= 105 && mode == true)
			{
				setcolor(4);
				color = 4;
				modecolor = true;
			}
			if (modecolor == true || mode_text == true)
			{
				c.linex = linex;
				c.liney = liney;
				c.color = color;
				c.font = textstyle;
				settextstyle(textstyle, 0, 1);
				k1.open(faux[active], ios::out | ios::in | ios::binary | ios::app);
				k1.write((char*)&c, sizeof(c));
				k1.close();
				modecolor = false;
				mode_text = false;
			}
			if (!(x >= 160 && x <= 210 && y >= 25 && y <= 35) && mode == true)
			{
				cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				f.close();
				setcolor(color);
				settextstyle(textstyle, 0, 1);				
				after_open_file(filename);
				mode = false;
			}

			char k[2];
			ch = getch();
			int modes = false;
			if (mode == true || mode_text == true)
				modes = true;
			if (mode == false)
			{
				k[0] = ch;
				k[1] = '\0';
				moveto(linex, liney);

				switch (ch)
				{
				case esc:
					f.close();
					exit(2);
					break;
					case '`':
					break;
					case 9:
					f.close();
					what_to_search(filename);
					cleardevice();
				draw_req_window_elements1();
				colourbox(color);
				setcolor(WHITE);
				char str[] = "C:\\TURBOC++\\";
				strcat(str, ftxt[active]);
				outtextxy(35, 5, str);
				setcolor(0);
				setcolor(color);
				settextstyle(textstyle, 0, 1);
				after_open_file(filename);
					break;
					case '~':
					if(underline_mode==false)
					{
						underline_mode=true;
						f.put('~');
					}
					else
					{
						underline_mode=false;
						f.put('~');
					}
					break;
				case up:
					liney = liney - spacey;
					pos = f.tellp();
					f.seekp(-pos, ios::cur);
					break;
				case down:
					liney = liney + spacey;
					pos = f.tellp();
					f.seekp(+pos, ios::cur);
					break;
				case left:
					linex = linex - spacex;
					pos = f.tellp();
					f.seekp(pos - 1);
					break;
				case right:
					linex = linex + spacex;
					pos = f.tellp();
					f.seekp(pos + 1);
					break;
				case enter:
					f.put('\n');
					linex = 30;
					liney = liney + spacey;
					if(b_mode)
				{
					draw_bullet(linex,liney);
					linex=40;
				}
					break;
				case bks:
				f.put('$');
				setfillstyle(1, 7);
				bar(linex-spacex,liney+spacey,linex,liney);
				linex=linex-spacex;
				break;
				default:
					outtextxy(linex, liney, "\0");
					f.put(ch);
					outtext(k);
					if(underline_mode==true)
					outtext("_");
					linex = linex + spacex;
				}
			}
		}


	}
};
class what_to_do :public openfile
{
public:
	void opt_what_to_do()
	{
		char c;
		outtextxy(50, 100, "1.NEW FILE");
		outtextxy(50, 125, "2.OPEN FILE");
		outtextxy(50, 170, "ENTER YOUR CHOICE:");
		moveto(200, 170);
		int done = 0;
		while (1 && done != 1)
		{
			close();
			c = getch();
			char k[2];
			k[0] = c;
			k[1] = '\0';
			outtext(k);
			if (c == '1')
			{
				draw_req_window_elements1();
				done = 1;
				name_of_openfile();
			}
			if (c == '2')
			{
				draw_req_window_elements1();
				done = 1;
				name_of_openfile();
			}
		}
	}
};
int main()
{
	int gdriver = DETECT, gmode, errorcode, status_mouse;
	status_mouse = init_mouse();
	//test variables
	int x, y, press, e = 0;
	initgraph(&gdriver, &gmode, "C:\\BGI\\");
	if (status_mouse == 0)
		cout << "Mouse not operational";
	else
		show_mouse();//mouse function

					 /* read result of initialization */
	errorcode = graphresult();

	if (errorcode != grOk)  /* an error occurred */
	{
		cout << "Graphics error:\n" << grapherrormsg(errorcode);
		cout << "Press any key to halt:";
		getch();
		exit(1);             /* return with error code */
	}
	for (int i = 0; i<5; ++i)
	{
		strcpy(ftxt[i], "untitled.txt");
	}
	loading l;
	l.load();
	openfile n;
	what_to_do w;
	n.draw_req_window_elements1();
	w.opt_what_to_do();
	
	getch();
	closegraph();
	return 0; }



