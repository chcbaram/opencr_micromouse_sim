#include "global.h"

char load_name[13]="NONAME.MAZ";
char name2[200][13];
unsigned char wall_info[256];
unsigned char visit_wall_info[256];
unsigned char mouse_real_japo;
unsigned char mouse_virtual_japo;
unsigned int dongo_map[256];
unsigned char path_trace[256];



int  MOUSE_REAL_DIR;
int  MOUSE_VIRTUAL_DIR;
unsigned  char mirohide[256];
int menu_status;
int miro_jido_cheak=0;
unsigned char GOAL=0x88;
unsigned char START_GOAL=0x00;
int MOUSE_SPEED=0;
int path_index;
int dongo_status=0;
int first_degak=0;
int speaker = 1;

const unsigned int wdir0[4] =   { 0x01, 0x02, 0x04, 0x08 };
const unsigned int wdir1[4] =   { 0x10, 0x20, 0x40, 0x80 };
const unsigned int unwdir[4] =  { 0x04, 0x08, 0x01, 0x02 };
const int dxy[4] =    { 0x01, 0x10, -1, -16 };
const int undxy[4] =  { -0x01, -0x10, 0x01, 0x10 };
const char hextable[17]="0123456789ABCDEF";
  int japo_conv[4][4]={ 0,1,2,3,
            1,2,3,0,
            2,3,0,1,
            3,0,1,2,
            };


struct cur {
  int cur_x;
  int cur_y[10];
  int expend;
} MENU_CUR;

void    InitTimer();
void    UninitTimer();

volatile int     timer2=0;
volatile int     flag2=0;
volatile int     flag1=0;
volatile int     timer1=0;
volatile int     so_index=0;
volatile int     timer3=0;
volatile int     one=10;


int x_step;
int y_step;
int temp_theta;
int mouse_dir;
int motor_status;
int gap_value;
double x_dim[11];
double y_dim[11];


#define BAN_BLOCK 5
#define MAX 17
#define MAX3 6


char degak[MAX][5] = {  {BUK,BUK,'e'},

            {BUK,DONG,DONG,BUK,'e'},
            {BUK,SU,SU,BUK,'e'},
            {BUK,DONG,DONG,SU,'e'},
            {BUK,SU,SU,DONG,'e'},
            {BUK,DONG,SU,'e'},
            {BUK,SU,DONG,'e'},
            {BUK,DONG,BUK,'e'},
            {BUK,SU,BUK,'e'},

            {SU,DONG,'e'},
            {SU,SU,DONG,'e'},
            {SU,SU,BUK,'e'},
            {SU,BUK,'e'},

            {DONG,SU,'e'},
            {DONG,DONG,SU,'e'},
            {DONG,DONG,BUK,'e'},
            {DONG,BUK,'e'}

             };
int scount[MAX] = {
  1,
  3,
  3,
  3,
  3,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  1,
  1,
  2,
  2,
  1
  };

char degak2[MAX3][5] ={
            {BUK,DONG,BUK,'e'},
            {BUK,SU,BUK,'e'},
            {BUK,DONG,DONG,BUK,'e'},
            {BUK,SU,SU,BUK,'e'},
            {BUK,DONG,SU,BUK,'e'},
            {BUK,SU,DONG,BUK,'e'}
          };

int count2[MAX3] = {
        2,
        2,
        3,
        3,
        3,
        3
          };

          
unsigned char B_buffer[256];
int B_index;


extern void setcolor(int color);
extern void setlinestyle(int a, int b, int c);
extern void line(int x0, int y0, int x1, int y1);
extern void outtextxy(int x, int y, char *p_str);


void init_m()
{
  int k;
  //double x_dim1[11] = {   0,13,0 ,-13,0 ,-4,4,-4,4,-2,2 };
  //double y_dim1[11] = { -13, 0,13,  0,0 ,-8,-8,8,8,-6,-6};
  double x_dim1[11] = {   0,MIRO_GAP/2,0 ,-MIRO_GAP/2,0 ,-3,3,-3,3,-1,1 };
  double y_dim1[11] = { -MIRO_GAP/2, 0,MIRO_GAP/2,  0,0 ,-5,-5,5,5,-3,-3};

  x_step = MIRO_X + MIRO_GAP/2;
  y_step = MIRO_Y - MIRO_GAP/2 + MIRO_GAP*16;

  for(k=0;k< 11;k++){
      x_dim[k] = x_step + x_dim1[k];
      y_dim[k] = y_step + y_dim1[k];
    }
  motor_status = MOTOR_START;
  temp_theta = 0;
  gap_value=0;
}

void goto_goal_error(int japo)
{

  int ch;

  while(mouse_real_japo!=japo)
  {
      if(graw_dongo(mouse_real_japo,japo,WALL)==0) break;

      make_path();
      trace();
//      miro_jido(1);

  }
}

void init_run()
{
  int i,ch;
  unsigned char real_dir,to_dir;

  for(i=0;i<256;i++){
    wall_info[i]&=0xf0;
    wall_info[i]|=0xf0;

    visit_wall_info[i]=NOVISIT;

    if((i/16)==0)
         wall_info[i]+=0x08;
    if((i/16)==15)
         wall_info[i]+=0x02;
    if((i%16)==0)
         wall_info[i]+=0x04;
    if((i%16)==15)
         wall_info[i]+=0x01;
  }
  visit_wall_info[0]=VISIT;

  
  
  set_block(0,DONG,0,15,wall_info);

  
  
  init_m();
  MOUSE_REAL_DIR=BUK;
  mouse_real_japo=START_GOAL;
  motor_status = MOTOR_START;
  draw_miro();
  wall_save();

}

void go_step(int dir, int step)
{
    int i,j;

       for(i=0; i<  step;i++)
       {
          body_e(x_dim,y_dim);

          switch(dir)
          {
                case 0:
                       for( j=0;j< 11;j++)  y_dim[j]--;
                       break;

                case 1:
           for(j=0;j< 11;j++)   x_dim[j]++;
          break;

                case 2:
                for(j=0;j< 11;j++)       y_dim[j]++;
          break;

                case 3:
                    for(j=0;j< 11;j++)       x_dim[j]--;
          break;
          }

        body(x_dim,y_dim);
          //delay(MOUSE_SPEED);
       }
}

void turn(int dir,int theta)
{
    double x1[11],y1[11];
    int ll,ll2,i,j;

      switch(dir)
      {
       case SU:
                temp_theta -= theta;  //기준 각도 에서.. 회전 각도를 뺀다
          ll2=dir;
          ll= -1;
          break;
           case DONG: //오른쪽회전일땐.. 오른쪽 점을 기준으로 돈다
                temp_theta += theta;
          ll2 =dir;
          ll = 1;
          break;
    case NAM:
                temp_theta += theta;
          ll2 = 4;
          ll =1;
          break;
    }
       for(i=0;i<11;i++)
       {
          x1[i] = (x_dim[i] - x_dim[ll2]);
          y1[i] = (y_dim[i]- y_dim[ll2]);
       }

       for( i=0; i< (theta/3)+1; i++)  //각도 만큼 회전 시킨다
       {

       if(first_degak != 1)
       body_e2(x_dim[ll2],y_dim[ll2],x1,y1);

     for( j=0; j< 11; j++)
           {
          x1[j] = ((x_dim[j] - x_dim[ll2])*cos(ll*con(i*3)) - (y_dim[j]- y_dim[ll2])*sin(ll*con(i*3)));
          y1[j] = ((x_dim[j] - x_dim[ll2])*sin(ll*con(i*3)) + (y_dim[j]- y_dim[ll2])*cos(ll*con(i*3)));
       }

       body2(x_dim[ll2],y_dim[ll2],x1,y1);
       //delay(MOUSE_SPEED);

       }

      for(i = 0;i< 11;i++)
      {
       x_dim[i] = x_dim[ll2]+x1[i];
       y_dim[i] = y_dim[ll2]+y1[i];
       }
        x_step = x_dim[4];
       y_step = y_dim[4];

}


void set_block(int japo,unsigned char dir,int ok,int color,unsigned char *miro)
{
    int x,y;

    y=MIRO_GAP * (15 - japo%16 ) + MIRO_Y;
    x=MIRO_GAP * (japo / 16) + MIRO_X;

    setcolor(color);
    setlinestyle(0,0,3);

    switch(dir)
    {
     case 0:
        if( (japo%16) != 15)
        {
           miro[japo]|=wdir0[dir];
           miro[japo]&=0xef;
           miro[japo+0x01]|=wdir0[NAM];
          miro[japo+0x01]&=0xbf;
        }
        if(ok==1)   line(x+2,y,x+MIRO_GAP-2,y);
        break;

     case 1:
        if( (japo/16) != 15)
        {
           miro[japo]|=wdir0[dir];
           miro[japo]&=0xdf;
           miro[japo+0x10]|=wdir0[SU];
           miro[japo+0x10]&=0x7f;
        }

        if(ok==1)   line(x+MIRO_GAP,y+2,x+MIRO_GAP,y+MIRO_GAP-2);
        break;

     case 2:
        if( (japo%16) != 0)
        {
           miro[japo]|=wdir0[dir];
           miro[japo]&=0xbf;
           miro[japo-0x01]|=wdir0[BUK];
           miro[japo-0x01]&=0xef;
        }
        if(ok==1)  line(x+2,y+MIRO_GAP,x+MIRO_GAP-2,y+MIRO_GAP);
        break;

     case 3:
        if( (japo/16) != 0 )
        {
           miro[japo]|=wdir0[dir];
           miro[japo]&=0x7f;
           miro[japo-0x10] |= wdir0[DONG];
           miro[japo-0x10] &= 0xdf;
        }
        if(ok==1) line(x,y+2,x,y+MIRO_GAP-2);
        break;
    }
   setcolor(15);
   setlinestyle(0,0,0);
}


void wall_save()
{
    int i;

    for(i=0;i<4;i++)
    {
     if( iswall0(mirohide[mouse_real_japo],i) )
       set_block(mouse_real_japo,i,1,15,wall_info);
    }
}


void draw_miro()
{
  int i,j,x,y,k;
  int wall_x[256],wall_y[256];

  k=0;
  for( i=0; i<16; i++)
  {
      for(j=0; j<16 ;j++) {

                   wall_x[k] =MIRO_X+MIRO_GAP*(i);
                   wall_y[k] =MIRO_Y+MIRO_GAP*(15-j);
                   k++;
            }
  }

  setlinestyle(0,0,3);

  for(j=0;j<256;j++) {

             for(i=0;i<4;i++) {

                 if(iswall0(mirohide[j],i)) {

                      setcolor(RED);
                      putpixel(wall_x[j],wall_y[j],YELLOW);
                      putpixel(wall_x[j]+MIRO_GAP,wall_y[j]+MIRO_GAP, YELLOW);
                      
                         switch(i) {
                             case 0:
                                      line(wall_x[j]+2,wall_y[j],wall_x[j]+MIRO_GAP-2,wall_y[j]);
                                      break;
                             case 1:
                                      line(wall_x[j]+MIRO_GAP,wall_y[j]+2,wall_x[j]+MIRO_GAP,wall_y[j]+MIRO_GAP-2);
                                      break;
                             case 2:
                                      line(wall_x[j]+2,wall_y[j]+MIRO_GAP,wall_x[j]+MIRO_GAP-2,wall_y[j]+MIRO_GAP);
                                      break;
                             case 3:
                                      line(wall_x[j],wall_y[j]+2,wall_x[j],wall_y[j]+MIRO_GAP-2);
                                      break;
                          }
                  }  else {
                         setcolor(BLACK);                         
                         switch(i) {
                                 case 0:
                                         line(wall_x[j]+2,wall_y[j],wall_x[j]+MIRO_GAP-2,wall_y[j]);
                                         break;
                                 case 1:
                                         line(wall_x[j]+MIRO_GAP,wall_y[j]+2,wall_x[j]+MIRO_GAP,wall_y[j]+MIRO_GAP-2);
                                         break;
                                 case 2:
                                         line(wall_x[j]+2,wall_y[j]+MIRO_GAP,wall_x[j]+MIRO_GAP-2,wall_y[j]+MIRO_GAP);
                                         break;
                                 case 3:
                                         line(wall_x[j],wall_y[j]+2,wall_x[j],wall_y[j]+MIRO_GAP-2);
                                         break;
                         }
                  }
             }
  }

  setcolor(15);
  setlinestyle(0,0,0);
  goal_print();
}


void goal_print()
{
   int x,y;

   y=MIRO_GAP*(15-GOAL%16)+MIRO_Y;
   x=MIRO_GAP*(GOAL/16)+MIRO_X;

   outtextxy(x+10,y+10,"G");

}


int graw_dongo(unsigned char START_JAPO,unsigned char GOAL_JAPO,int check)
{


  unsigned int i,j,index,sindex,mouseOK,x,y,mouse_OK;
  unsigned int buffer[256],save[256],mouse_POS,length;
  char *number="";
  unsigned char next_pos;


  number=(char *)malloc(4);

  mouse_OK=0;
  index=1;
  sindex=0;

  length=1;

  buffer[0]=GOAL_JAPO;

  mouse_POS=START_JAPO;

  for(i=0;i<256;i++) dongo_map[i] = 999;

  dongo_map[GOAL_JAPO]=0;

  if(dongo_status==1) {
    miro_jido(1);
    }
  while(1)
  {


  if(mouse_OK || !index) break;

     for(j=0;j<index;j++) {

      mouse_POS=buffer[j];


      if(dongo_map[mouse_POS] >= length){
               save[sindex]=mouse_POS;
               sindex++;
               continue;
               }

      for(i=0;i<4;i++)
      {
        if(iswall0(wall_info[mouse_POS],i)) continue;

        next_pos = next_location(mouse_POS,i);
        if(check == WALL)
          if(visit_wall_info[mouse_POS] == NOVISIT) continue;

        if(length >= dongo_map[next_pos]) continue;

        if(dongo_map[next_pos] > length) {

            save[sindex++]=next_pos;

           if(isbackwall(wall_info[mouse_POS],i))
            dongo_map[next_pos]=length+3;
           else{
            if(dongo_map[mouse_POS+undxy[i]]>=dongo_map[mouse_POS])
                dongo_map[next_pos]=length+3;
            else
                dongo_map[next_pos]=length;
            }

        }
      }

      if(mouse_POS == START_JAPO){ mouse_OK=1; break;  }
    }

    length++;

    index=sindex;
    for(i=0;i<sindex;i++) buffer[i]=save[i];
    sindex=0;
  }

  if(dongo_status==1) {
    setcolor(3);
    for(i=0;i<256;i++) {
      y=MIRO_GAP*(15-i%16)+MIRO_Y;
      x=MIRO_GAP*(i/16)+MIRO_X;

      number=itoa(dongo_map[i],number,10);
        outtextxy(x+2,y+10,number);
    }
    setcolor(15);
  }

    free(number);

    if(mouse_OK==1) return 1;
    else return 0;
}


void miro_jido(int select)
{

    int x,y,i;

    char number[4]="";
        setcolor(3);

        for(i=0;i<256;i++)
        {
      y=MIRO_GAP*(15-i%16)+MIRO_Y;
            x=MIRO_GAP*(i/16)+MIRO_X;

            number[1]=hextable[mirohide[i]%16];
            number[0]=hextable[mirohide[i]/16];
            number[2]=' ';

/* CHCBARAM    
            if(select==0)
                          outtextxy(x+2,y+10,number);
            else
                         bar2(x+2,y+2,x+24,y+24,BLACK);
*/                         
        }

     menu_status=1;
  setcolor(15);

}

int make_path()
{

  int index,vir_japo,i,vir_dir,vir_dir2,dir3;
  unsigned int length_buffer;
  unsigned char next_pos;

  length_buffer=dongo_map[mouse_real_japo];


  index=0;
  vir_japo=mouse_real_japo;
  vir_dir=MOUSE_REAL_DIR;

  for(i=0;i<256;i++) path_trace[i]=0;


  while(1){

     length_buffer=dongo_map[vir_japo];

     if(length_buffer==0 ) break;


     for(i=0;i<4;i++) {

      vir_dir2=(vir_dir+i)&3;
      next_pos = next_location(vir_japo,vir_dir2);

     if(!iswall0(wall_info[vir_japo],vir_dir2))
      if(length_buffer>dongo_map[next_pos])
      {
              length_buffer = dongo_map[next_pos];
              path_trace[index]=i;
              dir3=vir_dir2;
              //if(i==0) break;
      }
     }

     vir_japo+=dxy[dir3];
     vir_dir= dir3;

     index++;


     if(visit_wall_info[vir_japo]==NOVISIT) break;

     if(dongo_map[vir_japo] == 999) break;
   }

   path_index=index;
    if(visit_wall_info[vir_japo]==NOVISIT) return vir_japo;
   return 0;
}

int compare(char *source, char *dec)
{

   while(*source != 'e') { if(*source++ != *dec++) return 0;
        }
   return 1;
}

void dia_step(int step)
{
    int i,j;
    double kx=0,ky=0;
    double fstep;
    
    fstep = 16./26.;

    step = step;
    
    kx = sin(con(temp_theta));
    ky = cos(con(temp_theta));

    for(i = 0 ; i <  step ; i++)
    {
       if(i%10 == 0 || i==0)  body_e(x_dim,y_dim);

       if(kx>0 && ky >0)
       {
          for( j=0;j< 11;j++)
            {
                  y_dim[j]-=0.1*fstep;
                  x_dim[j]+=0.1*fstep;
      }
       }
       else if(kx<0 && ky > 0)
       {
            for(j=0;j< 11;j++)
          {
                     x_dim[j]-=0.1*fstep;
                     y_dim[j]-=0.1*fstep;
            }
        }
        else if(kx<0 && ky<0)
        {
              for(j=0;j< 11;j++)
              {
                         y_dim[j]+=0.1*fstep;
                         x_dim[j]-=0.1*fstep;
            }
        }
     else if(kx>0 && ky < 0)
        {
            for(j=0;j< 11;j++)
               {
                         x_dim[j]+=0.1*fstep;
                         y_dim[j]+=0.1*fstep;
               }
        } else if(kx==0 && ky==1)
         {
                for(j=0;j< 11;j++)
             {
                    y_dim[j]--;
             }
        } else if(kx==1 && ky==0)
        {
                for(j=0;j< 11;j++)
                {
                    x_dim[j]++;
       }
        } else if(kx==0 && ky== -1)
        {
                for(j=0;j< 11;j++)
             {
                     y_dim[j]++;
                }
        } else if(kx== -1 && ky==0)
        {
                for(j=0;j< 11;j++)
                {
                    x_dim[j]--;
                }
     }

       if(i%10 ==0)     body(x_dim,y_dim);
       //if(i%10 == 0)    delay2(MOUSE_SPEED);
       if(i%10 == 0)    body_e(x_dim,y_dim);
   }
}

void goto_degak_dongo(int japo)
{

  int ch;

  //GOAL=japo;
  while(mouse_real_japo != japo)
  {
      if(degak_dongo(mouse_real_japo,japo,UNWALL)==0) break;

      make_path();
      trace();
      //miro_jido(1);
  }
//  go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
}


int degak_dongo(unsigned char START_JAPO,unsigned char GOAL_JAPO,int check)
{


  unsigned int i,j,k,index,sindex,mouseOK,x,y,mouse_OK,temp_degak_dir;
  unsigned int buffer[256],save[256],save2[256],mouse_POS,length;
  char *number="";
  unsigned int next_pos,temp_dir,temp_POS,temp_value,temp_degak_dir2;
  unsigned int degak_map[256][4],temp_POS2[5];
  unsigned int degak_dir[256],visit_dongo[256];

  number=(char *)malloc(4);

  mouse_OK=0;
  index=1;
  sindex=0;

  length=0;

  buffer[0]=GOAL_JAPO;
  mouse_POS = GOAL_JAPO;

  for(i=0;i<4;i++){
     if(iswall0(wall_info[mouse_POS],i)) continue;
     degak_dir[0]=i;
     temp_POS2[i]=0;
     break;
     }
  for(j=0;j<256;j++) {
    visit_dongo[j] = NOVISIT;
    save2[j] = 10;
    for(i=0;i<4;i++) degak_map[j][i] = 999;

  }
  degak_map[GOAL_JAPO][degak_dir[0]] = 0;
  temp_POS = next_location(GOAL_JAPO,degak_dir[0]);
  degak_map[temp_POS][(degak_dir[0]+2)&0x03] = 1;
  save2[GOAL] = degak_dir[0];
  if(dongo_status==1) {
    miro_jido(1);
    }
  while(1)
  {

  if(mouse_OK || !index) break;

     for(j=0;j<index;j++) {


      mouse_POS=buffer[j];
      int temp1, temp2;

      if(degak_map[mouse_POS][0] < degak_map[mouse_POS][1])
        temp1=0;
      else temp1=1;
      if(degak_map[mouse_POS][2] < degak_map[mouse_POS][3])
        temp2=2;
      else temp2=3;
      if(degak_map[mouse_POS][temp1] <= degak_map[mouse_POS][temp2])
        temp_degak_dir = temp1;
      else temp_degak_dir = temp2;

      length = degak_map[mouse_POS][temp_degak_dir];
      /*
      temp_POS2= next_location(mouse_POS,temp_degak_dir);
      int temp_degak_dir2,temp_POS3;
      temp_degak_dir2 = save2[temp_POS2];
      temp_POS3 = next_location(temp_POS2,temp_degak_dir2);
      */
      temp_degak_dir2 = temp_degak_dir;
      temp_POS2[0] = mouse_POS;
      for(i=0;i<4;i++) {
        temp_POS2[i+1] = next_location(temp_POS2[i],temp_degak_dir2);
        temp_degak_dir2 = save2[temp_POS2[i+1]];
      }

      if(check == WALL)
        if(visit_wall_info[mouse_POS] == NOVISIT) continue;
      for(i=0;i<4;i++)
      {
        temp_dir = (temp_degak_dir+i)&0x03;

        if(iswall0(wall_info[mouse_POS],temp_dir)) continue;

        temp_POS = next_location(mouse_POS,temp_dir);

        switch(i){
          case 1:

          case  3:
            if(degak_map[mouse_POS][temp_dir] <= (length+4)) break;
              degak_map[mouse_POS][temp_dir] = length + 4;
            if(((temp_dir+2)&0x03) == save2[temp_POS2[3]])     // 90t
                if(((temp_degak_dir+2)&0x03) == save2[temp_POS2[2]])
                degak_map[mouse_POS][temp_dir] += 4;
            if(((temp_dir+2)&0x03) == save2[temp_POS2[1]])
               if(temp_degak_dir ==save2[temp_POS2[2]])
               if(((temp_dir+2)&0x03) == save2[temp_POS2[3]])
                degak_map[mouse_POS][temp_dir] -= 2;
            if(temp_degak_dir == save2[temp_POS2[1]])
                degak_map[mouse_POS][temp_dir] += 0;


            /*
            if(save2[temp_POS2] == temp_dir) {
               if(save2[temp_POS2+dxy[temp_dir]] != temp_dir) //90t
                 degak_map[mouse_POS][temp_dir] +=3;
               else
                 degak_map[mouse_POS][temp_dir] +=4;

               if(save2[temp_POS2+dxy[temp_dir]] == temp_dir)  //uturn
                  if(iswall0(wall_info[temp_POS],(temp_degak_dir+2)&0x03))
                  degak_map[mouse_POS][temp_dir] +=5;

            }  else if(save2[temp_POS2+dxy[(temp_dir+2)&0x03]] ==temp_degak_dir)  // dega_go
                 degak_map[mouse_POS][temp_dir] -=3;
            */
            if(check == WALL)
            if(visit_wall_info[temp_POS] == NOVISIT) break;

            degak_map[temp_POS][(temp_dir+2)&0x03] = length+4;
            if(((temp_degak_dir+2)&0x03) == save2[temp_POS2[2]])  // 90t
              if(((temp_dir+2)&0x03) == save2[temp_POS2[3]])
               degak_map[temp_POS][(temp_dir+2)&0x03] += 4;
            if(((temp_dir+2)&0x03) == save2[temp_POS2[1]])
               if(temp_degak_dir ==save2[temp_POS2[2]])
               if(((temp_dir+2)&0x03) == save2[temp_POS2[3]])
                 degak_map[temp_POS][(temp_dir+2)&0x03] -= 2;
             if(temp_degak_dir == save2[temp_POS2[1]])
                  degak_map[temp_POS][(temp_dir+2)&0x03] += 0;
            /*
            if(save2[temp_POS2] == temp_dir) {
             if(save2[temp_POS2+dxy[temp_dir]] != temp_dir)   //90t
                degak_map[temp_POS][(temp_dir+2)&0x03]+=3;
             else
                degak_map[temp_POS][(temp_dir+2)&0x03]+=4;

             if(save2[temp_POS2+dxy[temp_dir]] == temp_dir)   //uturn
              if(iswall0(wall_info[temp_POS],(temp_degak_dir+2)&0x03))
                degak_map[temp_POS][(temp_dir+2)&0x03]+=5;
             } else if(save2[temp_POS2+dxy[(temp_dir+2)&0x03]] ==temp_degak_dir)  // dega_go
                  degak_map[temp_POS][(temp_dir+2)&0x03]-=3;
             */
            break;

          case 2:
            if(degak_map[mouse_POS][temp_dir] <= (length+6)) break;
              degak_map[mouse_POS][temp_dir] = length + 6;
            if(temp_dir == save2[temp_POS2[2]])        // uturn
                if(temp_dir == save2[temp_POS2[3]])
                degak_map[mouse_POS][temp_dir]+= 6;

            if(temp_degak_dir == save2[temp_POS2[1]])     // go
               if(temp_degak_dir == save2[temp_POS2[2]])
              if(temp_degak_dir == save2[temp_POS2[3]])
                degak_map[mouse_POS][temp_dir] -= 3;
            if(temp_degak_dir != save2[temp_POS2[1]])      // 90
               if(save2[temp_POS2[1]] == save2[temp_POS2[2]])
               degak_map[mouse_POS][temp_dir] += 3;


            //if(temp_degak_dir == save2[temp_POS2[2]])   //45
             // if(temp_degak_dir == save2[temp_POS2[3]])
              //     degak_map[mouse_POS][temp_dir] += 2;


            /*
            if(save2[temp_POS2] == temp_degak_dir)
              degak_map[mouse_POS][temp_dir] -=3;  // go
            else if(save2[temp_POS3] == save2[temp_POS2])
              degak_map[mouse_POS][temp_dir] +=3;  // 90
            */
            if(check == WALL)
            if(visit_wall_info[temp_POS] == NOVISIT) break;
              degak_map[temp_POS][(temp_dir+2)&0x03] = length+6;
            if(temp_dir == save2[temp_POS2[3]])        //uturn
                if(temp_dir == save2[temp_POS2[2]])
                degak_map[temp_POS][(temp_dir+2)&0x03] +=6;

            if(temp_degak_dir == save2[temp_POS2[1]])    // go
               if(temp_degak_dir == save2[temp_POS2[2]])
              if(temp_degak_dir == save2[temp_POS2[3]])
              degak_map[temp_POS][(temp_dir+2)&0x03] -=3;
            if(temp_degak_dir != save2[temp_POS2[1]])      // 90
               if(save2[temp_POS2[1]] == save2[temp_POS2[2]])
               degak_map[temp_POS][(temp_dir+2)&0x03] +=3;

            //if(temp_degak_dir == save2[temp_POS2[2]]) // 45
             // if(temp_degak_dir == save2[temp_POS2[3]])
              //        degak_map[temp_POS][(temp_dir+2)&0x03] +=2;
            /*
            if(save2[temp_POS2] == temp_degak_dir)
              degak_map[temp_POS][(temp_dir+2)&0x03] -=3;   //go
            else if(save2[temp_POS3] == save2[temp_POS2])  //90
              degak_map[temp_POS][(temp_dir+2)&0x03] +=3;
            */
            break;
        }
        visit_dongo[mouse_POS] = VISIT;
        save2[mouse_POS] = temp_degak_dir;
        if(visit_dongo[temp_POS] == VISIT) continue;
        else
        {
          save[sindex++]=temp_POS;
          //save2[sindex++]=k;
          visit_dongo[temp_POS]=VISIT;
        }
      }
      if(mouse_POS == START_JAPO){ mouse_OK=1; break;  }
    }

    index=sindex;
    for(i=0;i<sindex;i++){
       buffer[i]=save[i];
       //degak_dir[i]=save2[i];
    }
    sindex=0;
  }
  for(i=0;i<256;i++) {
      temp_value = degak_map[i][1];
      if(degak_map[i][0] <= temp_value)
        temp_value = degak_map[i][0];
      if(degak_map[i][2] <= temp_value)
        temp_value = degak_map[i][2];
      if(degak_map[i][3] <= temp_value)
        temp_value = degak_map[i][3];
      dongo_map[i] = temp_value;
  }
  if(dongo_status==1) {
    setcolor(3);
    for(i=0;i<256;i++) {
      y=MIRO_GAP*(15-i%16)+MIRO_Y;
      x=MIRO_GAP*(i/16)+MIRO_X;

      number=itoa(dongo_map[i],number,10);
        outtextxy(x+2,y+10,number);
    }
    setcolor(15);
  }

    free(number);

    if(mouse_OK==1) return 1;
    else return 0;
}


void compilpath()
{
  int i,j;

  B_index = 0;
  path_trace[path_index] = 0;

  for(i = 0; i< path_index;)
  {
    for(j=0; j< MAX; j++)
       if(compare(degak[j],(char *)(path_trace + i)))
       {
      B_buffer[B_index++] = (unsigned char)j;
      i += scount[j];
      break;
        }
   }
}


void compilpath2()
{
  int i,j,k,k2,m_s;

  B_index = 0;
  k2=0;
  path_trace[path_index] = 10;
  m_s = motor_status;
  for(i = 0; i< path_index;)
  {
    k = B_index;
    for(j=0; j< 4; j++) {
       if(compare(degak2[j],(char *)(path_trace + i)))
       {
      if(m_s == BAN_BLOCK ){
        path_trace[B_index++] = 15;
        m_s = MOTOR_CONTINUE;
        }

      path_trace[B_index++] = (unsigned char)j+4;
      k2 = j+4;
      i += count2[j];
      break;
        }
     }
     if(B_index == k){
            if(k2>0){
            i++;
            k2=0;
            m_s = BAN_BLOCK;
            } else {
            path_trace[B_index++] = path_trace[i];
            i++;
            if(m_s == BAN_BLOCK)
              m_s = MOTOR_CONTINUE;
            }
      }
   }

   path_index = B_index;
}



void trace()
{
    unsigned char vir_dir,trace_dir;
    unsigned int index,i,j,temp,go_step1,block;
    int an[4] = { NAM,SU,BUK,DONG },cnt;
    double x_t[5];
    double y_t[5];

    char *path_t;

    path_t = (char *)malloc(1);
    index=0;
    

    compilpath2();
    for(i=0;i < path_index; i++)
    {
     j = path_trace[i];
     itoa(j,path_t,10);
     //outtextxy(480+(i%11)*10,222+(i/11)*10,path_t);
    }
    free(path_t);
  //   compilpath2();

    //if(first_degak == 1)

    do
    {

     vir_dir=path_trace[index];
     /*trace_dir=japo_conv[MOUSE_REAL_DIR][vir_dir];*/
     trace_dir=(MOUSE_REAL_DIR+vir_dir)&3;

    /* if(compare(degak[0],(char *)(path_trace+index))) printf("c");*/


     switch(vir_dir) {
      case BUK:
        for(i=0;;i++,index++) {
          if(path_trace[index]!=vir_dir) break;
          if(index>=path_index) break;
        }

        temp=MOUSE_SPEED;
        if(motor_status == BAN_BLOCK)  {
          block = MIRO_GAP/2;
          motor_status = MOTOR_CONTINUE;
          }
        else
          block = 0;

        go_step1 = (i-1)*MIRO_GAP;
              if(motor_status == MOTOR_START)
              {
          go_step(trace_dir,MIRO_GAP/2);
          if(i>1) go_step(trace_dir,go_step1);
        } else if(motor_status == MOTOR_CONTINUE | motor_status ==100){
          if(i==1) go_step(trace_dir,MIRO_GAP+block);
          else{ MOUSE_SPEED = temp-1;
             go_step(trace_dir,go_step1+MIRO_GAP+block);
              }
        }

              for(j=0;j<i;j++)
              {
                  if(i>2)
                  MOUSE_SPEED=temp-2;
                  //ani_draw_mouse(trace_dir,mouse_real_japo+dxy[trace_dir]);
                  //go_step(trace_dir,27);
                  mouse_real_japo+=dxy[trace_dir];
              }


        MOUSE_SPEED=temp;
        MOUSE_REAL_DIR=trace_dir;
              index--;
        motor_status = MOTOR_CONTINUE;
        break;
      case NAM:
          if(motor_status == BAN_BLOCK)
           go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        temp=MOUSE_SPEED;
        MOUSE_SPEED +=4;
        if(motor_status == 100){

        switch(MOUSE_REAL_DIR) {
            case 0:
                 gap_value = (MIRO_Y+ (MIRO_GAP*(16-GOAL%16)))-(int)y_dim[4];
                 break;
            case  2:
                 gap_value = (int)y_dim[4] - (MIRO_Y+ (MIRO_GAP*(15-GOAL%16)));
                 break;
            case  1:
                 gap_value = x_dim[4] - (MIRO_X+(MIRO_GAP*(GOAL/16)));
                 break;
            case  3:
                 gap_value = (MIRO_X+(MIRO_GAP*(GOAL/16+1))) - x_dim[4];
                 break;
           }
        }
        go_step(MOUSE_REAL_DIR,MIRO_GAP/2-gap_value);
        gap_value=0;
        MOUSE_SPEED = temp;
        turn(NAM,180);
        go_step(trace_dir,MIRO_GAP/2);
        mouse_real_japo+=dxy[trace_dir];
        //ani_draw_mouse(trace_dir,mouse_real_japo);
        MOUSE_REAL_DIR=trace_dir;
        motor_status = MOTOR_CONTINUE;
        break;
      case DONG:
        if(motor_status == BAN_BLOCK)
        if(motor_status != 100)
           go_step(MOUSE_REAL_DIR,MIRO_GAP/2);

        mouse_real_japo+=dxy[trace_dir];
        //ani_draw_mouse(trace_dir,mouse_real_japo);
        turn(DONG,90);
        if(speaker == 1) flag1=1;
        MOUSE_REAL_DIR=trace_dir;
        motor_status = MOTOR_CONTINUE;
        break;
      case SU:
        if(motor_status == BAN_BLOCK)
        if(motor_status != 100)
           go_step(MOUSE_REAL_DIR,MIRO_GAP/2);

        mouse_real_japo+=dxy[trace_dir];
        //ani_draw_mouse(trace_dir,mouse_real_japo);
        turn(SU,90);
        if(speaker == 1) flag1=1;
        MOUSE_REAL_DIR=trace_dir;
        motor_status = MOTOR_CONTINUE;
        break;
      case 4:   /* R90t */
        //trace_dir=(MOUSE_REAL_DIR+BUK)&3;
        first_degak = 1;
        cnt = 2;
        if(path_trace[index +1] < 4) cnt = 3;
        if(path_trace[index +1] == 15) cnt = 3;
        if(motor_status == BAN_BLOCK || motor_status == MOTOR_START)
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        else if(motor_status == MOTOR_CONTINUE)
          go_step(MOUSE_REAL_DIR,MIRO_GAP);

        turn(DONG,90);

      for(j=0;j<cnt;j++) {
        vir_dir = (MOUSE_REAL_DIR + degak2[0][j])&3;
        MOUSE_REAL_DIR = vir_dir;
        mouse_real_japo += dxy[vir_dir];
      }

          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
          first_degak = 0;
          motor_status = BAN_BLOCK;
      break;

      case 5:   /* L90t */
        cnt = 2;
        first_degak = 1;
        if(path_trace[index +1] < 4) cnt = 3;
        if(path_trace[index +1] == 15) cnt = 3;
        if(motor_status == BAN_BLOCK || motor_status == MOTOR_START)
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        else
          go_step(MOUSE_REAL_DIR,MIRO_GAP);

        turn(SU,90);
      for(j=0;j<cnt;j++) {
        vir_dir = (MOUSE_REAL_DIR + degak2[1][j])&3;
        MOUSE_REAL_DIR = vir_dir;
        mouse_real_japo += dxy[vir_dir];
      }

          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
          first_degak = 0;
          motor_status = BAN_BLOCK;
      break;

      case 6:     /* R180t */

        cnt = 3;
        first_degak = 1;
        if(path_trace[index +1] < 4) cnt = 4;
        if(path_trace[index +1] == 15) cnt = 4;
        if(path_trace[index +1] == 10) cnt = 4;
        if(motor_status == BAN_BLOCK || motor_status == MOTOR_START)
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        else
          go_step(MOUSE_REAL_DIR,MIRO_GAP);

        turn(DONG,180);
      for(j=0;j<cnt;j++) {
        vir_dir = (MOUSE_REAL_DIR + degak2[2][j])&3;
        MOUSE_REAL_DIR = vir_dir;
        mouse_real_japo += dxy[vir_dir];
      }

          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
          first_degak = 0;
          motor_status = BAN_BLOCK;
      break;

      case 7:    /* L180t */
        cnt = 3;
        first_degak = 1;
        if(path_trace[index +1] < 4) cnt = 4;
        if(path_trace[index +1] == 15) cnt = 4;
        if(path_trace[index +1] == 10) cnt = 4;

        if(motor_status == BAN_BLOCK || motor_status == MOTOR_START)
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        else
          go_step(MOUSE_REAL_DIR,MIRO_GAP);

        turn(SU,180);
      for(j=0;j<cnt;j++) {
        vir_dir = (MOUSE_REAL_DIR + degak2[3][j])&3;
        MOUSE_REAL_DIR = vir_dir;
        mouse_real_japo += dxy[vir_dir];
      }

          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
          motor_status = BAN_BLOCK;
          first_degak = 0;
      break;

      case 15:
        go_step(MOUSE_REAL_DIR,MIRO_GAP);
        break;

     }

     if( visit_wall_info[mouse_real_japo] == NOVISIT)
     {
          visit_wall_info[mouse_real_japo]=VISIT;
          wall_save();
     }

     index++;

    } while( index < path_index );

}


void first_run()
{
  init_run();


  //draw_mouse(0,0);

  graw_dongo(mouse_real_japo,GOAL,UNWALL);

  goto_goal_dongo(GOAL);


  graw_dongo(mouse_real_japo,START_GOAL,UNWALL);


//  find_path(1);
//  find_path(0);

//  if(visit_wall_info[0x78]==NOVISIT) goto_goal_dongo(0x78);
//  if(visit_wall_info[0x77]==NOVISIT) goto_goal_dongo(0x77);
//  if(visit_wall_info[0x87]==NOVISIT) goto_goal_dongo(0x87);

  goto_goal_dongo(0x00);
}


void goto_goal_dongo(int japo)
{

  int ch;

  //GOAL=japo;
  while(mouse_real_japo != japo)
  {
      if(graw_dongo(mouse_real_japo,japo,UNWALL)==0) break;

      make_path();
      trace();
      //miro_jido(1);

      //if(bioskey(1)!=0) { ch=bioskey(0);
      //        if(ch==ESC) break;
      //      }
  }
//  go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
}

void deka_trace()
{
  int i,j,vir_dir,k,g_step;

  char *pp;
  pp=(char *)malloc(1);


  compilpath();


      for(i=0;i < B_index; i++)
          {
           j = B_buffer[i];
           itoa(j,pp,10);
           //outtextxy(479+(i%6)*20,220+(i/6)*10,pp);
           }
  free(pp);

  for(i=0; i< B_index; i++)
  {
       vir_dir = MOUSE_REAL_DIR;
       for(j=0;j<scount[B_buffer[i]];j++)
       {
          vir_dir = (vir_dir + degak[B_buffer[i]][j])&3;
       }
       switch(B_buffer[i])
       {
      case  0:

        if(motor_status==MOTOR_START) {
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
          motor_status = BAN_BLOCK;
        } else if(motor_status == MOTOR_CONTINUE){
             if(i != B_index-1)
              go_step(MOUSE_REAL_DIR,MIRO_GAP);
             else
              go_step(MOUSE_REAL_DIR,MIRO_GAP/2);

             motor_status = MOTOR_CONTINUE;
        } else if(motor_status == BAN_BLOCK) {
            go_step(MOUSE_REAL_DIR,MIRO_GAP/2+MIRO_GAP);
            motor_status = MOTOR_CONTINUE;
            }
      break;

      case  1:  /* rturn180(); */

         if(motor_status == BAN_BLOCK)
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2);

         go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
         turn(DONG,180);
         if(i != B_index-1)
            go_step(vir_dir,MIRO_GAP/2);
         motor_status = MOTOR_CONTINUE;
         break;
      case  2:  /* lturn180(); */
         go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
         turn(SU,180);
         if(i != B_index-1)
          go_step(vir_dir,MIRO_GAP/2);
         motor_status = MOTOR_CONTINUE;
         break;
      case  3:  /* enter_rturn135() */
         go_step(MOUSE_REAL_DIR,8);
         turn(DONG,135);
         dia_step(40);
         motor_status = MOTOR_CONTINUE;
         break;
      case  4:  /* enter_lturn135(); */
          go_step(MOUSE_REAL_DIR,8);
         turn(SU,135);
         dia_step(40);
         motor_status = MOTOR_CONTINUE;
         break;
      case  5:  /*enter_rturn45(); */
        if(motor_status == BAN_BLOCK) {
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2+8);
        } else {
          go_step(MOUSE_REAL_DIR,8);
          }
          turn(DONG,45);
          dia_step(92);
         motor_status = MOTOR_CONTINUE;
         break;
      case  6: /* enter_lturn45() */
         if(motor_status == BAN_BLOCK) {
          go_step(MOUSE_REAL_DIR,MIRO_GAP/2+8);
        } else {
          go_step(MOUSE_REAL_DIR,8);
          }
          turn(SU,45);
          dia_step(92);
          motor_status = MOTOR_CONTINUE;
         break;
      case  7:  /* rturn90() */
        if(motor_status == BAN_BLOCK)
             go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        turn(DONG,90);
        if(i != B_index-1)
           go_step(vir_dir,MIRO_GAP/2);
        motor_status = MOTOR_CONTINUE;
        break;
      case  8:  /* lturn90()*/
        go_step(MOUSE_REAL_DIR,MIRO_GAP/2);
        turn(SU,90);
        if(i != B_index-1)
           go_step(vir_dir,MIRO_GAP/2);
        motor_status = MOTOR_CONTINUE;
        break;
      case  9:   /* go45() */
         dia_step(130);
         motor_status = MOTOR_CONTINUE;
         break;
      case 10: /* lturn90t() */
         dia_step(38);
         turn(SU,90);
         dia_step(38);
         motor_status = MOTOR_CONTINUE;
         break;
      case 11: /* out_lturn135() */
         dia_step(27);
         turn(SU,135);
         if(i != B_index-1)
          go_step(vir_dir,7);
         motor_status = MOTOR_CONTINUE;
         break;
      case 12: /* out_lturn45() */
          dia_step(90);
          turn(SU,45);
          if(i != B_index-1)
          go_step(vir_dir,13 - 5);
          motor_status = MOTOR_CONTINUE;
          break;

      case 13: /*go 45() */
        dia_step(130);
        motor_status = MOTOR_CONTINUE;
        break;
      case 14:/* rturn90t() */
        dia_step(38);
        turn(DONG,90);
        dia_step(38);
        motor_status = MOTOR_CONTINUE;
        break;
      case 15:  /* out_rturn135();*/
        dia_step(27);
        turn(DONG,135);
        if(i != B_index-1)
            go_step(vir_dir,7);
        motor_status = MOTOR_CONTINUE;
        break;
      case 16:  /* out_rturn45() */
          dia_step(90);
          turn(DONG,45);
          if(i != B_index-1)
           go_step(vir_dir,MIRO_GAP/2 - 5);
          motor_status = MOTOR_CONTINUE;
          break;
    }
    for(j=0;j<scount[B_buffer[i]];j++) {
      vir_dir = (MOUSE_REAL_DIR + degak[B_buffer[i]][j])&3;
      MOUSE_REAL_DIR = vir_dir;
      mouse_real_japo += dxy[vir_dir];
    }
   }
   motor_status = 100;
}


void deka_run()
{
  mouse_real_japo=START_GOAL;
  MOUSE_REAL_DIR=BUK;
  init_m();

  degak_dongo(mouse_real_japo,GOAL,WALL);

  make_path();
  deka_trace();


  goto_degak_dongo(START_GOAL);
}

void second_run()
{

  mouse_real_japo=0x00;
  MOUSE_REAL_DIR=BUK;
  init_m();

  //draw_mouse(0,0);
  graw_dongo(mouse_real_japo,GOAL,UNWALL);

  goto_goal_error(GOAL);


  goto_goal_dongo(0x00);
}

