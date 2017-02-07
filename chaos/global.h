#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "Waveshare_HX8347D.h"

#define MIRO_GAP  (14)
#define MIRO_X    ((240-(MIRO_GAP*16))/2)
#define MIRO_Y    ((320-(MIRO_GAP*16))/2)
#define MOUSE_WIDTH   6

#define BUK       0
#define DONG	    1
#define NAM	      2
#define SU	      3
#define NOVISIT   0
#define VISIT	    1
#define NEMOUSE   4
#define NWMOUSE   5
#define SEMOUSE   6
#define SWMOUSE   7
#define UNWALL    0
#define WALL      1
#define MOTOR_START 0
#define MOTOR_CONTINUE 1

#define con(x)  (x*M_PI)/180
#define BD 1
#define BS 2
#define ND 3
#define NS 4

#define RED    HX8347D_RED
#define BLACK  HX8347D_BLACK
#define WHITE  HX8347D_WHITE
#define YELLOW HX8347D_YELLOW

#define iswall0( wall,j)			(wall & wdir0[j])		/*	0:North 1:East 2:South 3:West	*/
#define iswall1(wall,j)            ((wall & 0xf0) & wdir1[j])
#define isbackwall( wall, jdir)		(wall & unwdir[jdir])
#define next_location( pos, jdir)	(pos + dxy[jdir])
#define back_location( pos, jdir)	(pos + undxy[jdir])



void new_miro(int st_num);
void erase_mouse(int japo);
void bar2(int x,int y,int x2,int y2,int color);
void draw_miro_edit(int japo,int dir);
void clear_miro_value();
void Save(void);
void miro_save();
char get_key(int ke);
int return_yes(int x,int y);
void set_block(int japo,unsigned char dir,int ok,int color,unsigned char *miro);
void batan_miro();
void menu_main();
void bol_box(int x,int y,int x2,int y2,int color);
void batan_main_menu(int num,int size,int color);
void miro_load();
void graw_file(int num,int size,int count);
void Load_miro(char  *fname);
void draw_miro();
void LOAD_IMAGE(char *t_name,int base_x,int base_y,int x_size,int y_size);
void draw_mouse(int dir,int japo);
void miro_jido(int select);
int conv(char x[3]);
void wall_save();
void goto_goal_left(int japo);
void bol_box2(int x,int y,int x2,int y2,int in_color,int out_color);
void logo_baram();
void logo_maker();
void mouse_speed();
int graw_dongo(unsigned char START_JAPO,unsigned char GOAL_JAPO,int check);
int graw_dongo2(unsigned char START_JAPO,unsigned char GOAL_JAPO);
int degak_dongo(unsigned char START_JAPO,unsigned char GOAL_JAPO,int check);

int make_path();
void trace();
void goto_goal_dongo(int japo);
void goto_degak_dongo(int japo);
void goto_goal_error(int japo);
void init_run();
void first_run();
void first_run2();
void second_run();
void second_run2();
unsigned char next_l(unsigned char japo,int dir);
void goal_print();
void ani_draw_mouse(int dir,int japo);
void serch_path();
void bol_box_2(int x,int y ,int x2,int y2,int color);
void delay2(int x);



void go_step(int dir, int step);
void turn(int dir,int theta);
void dia_step(int step);


extern unsigned  char mirohide[256];
extern double x_dim[11];
extern double y_dim[11];

void body(double *xx1,double *yy1);
void body_e(double *xx2,double *yy2);
void body_e2(double x,double y,double *xx4,double *yy4);
void body2(double x,double y,double *xx3,double *yy3 );
void deka_run();
void second_run();
void putpixel(int x, int y, int color);
#endif
