/* FFdecsa -- fast decsa algorithm
 *
 * Copyright (C) 2003-2004  fatih89r
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


struct group_t{
  unsigned char s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16;
};
typedef struct group_t group;

#define GROUP_PARALLELISM 128

inline static group FF0(){
  group res;
  res.s1=0x0;
  res.s2=0x0;
  res.s3=0x0;
  res.s4=0x0;
  res.s5=0x0;
  res.s6=0x0;
  res.s7=0x0;
  res.s8=0x0;
  res.s9=0x0;
  res.s10=0x0;
  res.s11=0x0;
  res.s12=0x0;
  res.s13=0x0;
  res.s14=0x0;
  res.s15=0x0;
  res.s16=0x0;
  return res;
}

inline static group FF1(){
  group res;
  res.s1=0xff;
  res.s2=0xff;
  res.s3=0xff;
  res.s4=0xff;
  res.s5=0xff;
  res.s6=0xff;
  res.s7=0xff;
  res.s8=0xff;
  res.s9=0xff;
  res.s10=0xff;
  res.s11=0xff;
  res.s12=0xff;
  res.s13=0xff;
  res.s14=0xff;
  res.s15=0xff;
  res.s16=0xff;
  return res;
}

inline static group FFAND(group a,group b){
  group res;
  res.s1=a.s1&b.s1;
  res.s2=a.s2&b.s2;
  res.s3=a.s3&b.s3;
  res.s4=a.s4&b.s4;
  res.s5=a.s5&b.s5;
  res.s6=a.s6&b.s6;
  res.s7=a.s7&b.s7;
  res.s8=a.s8&b.s8;
  res.s9=a.s9&b.s9;
  res.s10=a.s10&b.s10;
  res.s11=a.s11&b.s11;
  res.s12=a.s12&b.s12;
  res.s13=a.s13&b.s13;
  res.s14=a.s14&b.s14;
  res.s15=a.s15&b.s15;
  res.s16=a.s16&b.s16;
  return res;
}

inline static group FFOR(group a,group b){
  group res;
  res.s1=a.s1|b.s1;
  res.s2=a.s2|b.s2;
  res.s3=a.s3|b.s3;
  res.s4=a.s4|b.s4;
  res.s5=a.s5|b.s5;
  res.s6=a.s6|b.s6;
  res.s7=a.s7|b.s7;
  res.s8=a.s8|b.s8;
  res.s9=a.s9|b.s9;
  res.s10=a.s10|b.s10;
  res.s11=a.s11|b.s11;
  res.s12=a.s12|b.s12;
  res.s13=a.s13|b.s13;
  res.s14=a.s14|b.s14;
  res.s15=a.s15|b.s15;
  res.s16=a.s16|b.s16;
  return res;
}

inline static group FFXOR(group a,group b){
  group res;
  res.s1=a.s1^b.s1;
  res.s2=a.s2^b.s2;
  res.s3=a.s3^b.s3;
  res.s4=a.s4^b.s4;
  res.s5=a.s5^b.s5;
  res.s6=a.s6^b.s6;
  res.s7=a.s7^b.s7;
  res.s8=a.s8^b.s8;
  res.s9=a.s9^b.s9;
  res.s10=a.s10^b.s10;
  res.s11=a.s11^b.s11;
  res.s12=a.s12^b.s12;
  res.s13=a.s13^b.s13;
  res.s14=a.s14^b.s14;
  res.s15=a.s15^b.s15;
  res.s16=a.s16^b.s16;
  return res;
}

inline static group FFNOT(group a){
  group res;
  res.s1=~a.s1;
  res.s2=~a.s2;
  res.s3=~a.s3;
  res.s4=~a.s4;
  res.s5=~a.s5;
  res.s6=~a.s6;
  res.s7=~a.s7;
  res.s8=~a.s8;
  res.s9=~a.s9;
  res.s10=~a.s10;
  res.s11=~a.s11;
  res.s12=~a.s12;
  res.s13=~a.s13;
  res.s14=~a.s14;
  res.s15=~a.s15;
  res.s16=~a.s16;
  return res;
}


/* 64 rows of 128 bits */

inline static void FFTABLEIN(unsigned char *tab, int g, unsigned char *data){
  *(((int *)tab)+2*g)=*((int *)data);
  *(((int *)tab)+2*g+1)=*(((int *)data)+1);
}

inline static void FFTABLEOUT(unsigned char *data, unsigned char *tab, int g){
  *((int *)data)=*(((int *)tab)+2*g);
  *(((int *)data)+1)=*(((int *)tab)+2*g+1);
}

inline static void FFTABLEOUTXORNBY(int n, unsigned char *data, unsigned char *tab, int g){
  int j;
  for(j=0;j<n;j++){
    *(data+j)^=*(tab+8*g+j);
  }
}


struct batch_t{
  unsigned char s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16;
};
typedef struct batch_t batch;

#define BYTES_PER_BATCH 16

inline static batch B_FFAND(batch a,batch b){
  batch res;
  res.s1=a.s1&b.s1;
  res.s2=a.s2&b.s2;
  res.s3=a.s3&b.s3;
  res.s4=a.s4&b.s4;
  res.s5=a.s5&b.s5;
  res.s6=a.s6&b.s6;
  res.s7=a.s7&b.s7;
  res.s8=a.s8&b.s8;
  res.s9=a.s9&b.s9;
  res.s10=a.s10&b.s10;
  res.s11=a.s11&b.s11;
  res.s12=a.s12&b.s12;
  res.s13=a.s13&b.s13;
  res.s14=a.s14&b.s14;
  res.s15=a.s15&b.s15;
  res.s16=a.s16&b.s16;
  return res;
}

inline static batch B_FFOR(batch a,batch b){
  batch res;
  res.s1=a.s1|b.s1;
  res.s2=a.s2|b.s2;
  res.s3=a.s3|b.s3;
  res.s4=a.s4|b.s4;
  res.s5=a.s5|b.s5;
  res.s6=a.s6|b.s6;
  res.s7=a.s7|b.s7;
  res.s8=a.s8|b.s8;
  res.s9=a.s9|b.s9;
  res.s10=a.s10|b.s10;
  res.s11=a.s11|b.s11;
  res.s12=a.s12|b.s12;
  res.s13=a.s13|b.s13;
  res.s14=a.s14|b.s14;
  res.s15=a.s15|b.s15;
  res.s16=a.s16|b.s16;
  return res;
}

inline static batch B_FFXOR(batch a,batch b){
  batch res;
  res.s1=a.s1^b.s1;
  res.s2=a.s2^b.s2;
  res.s3=a.s3^b.s3;
  res.s4=a.s4^b.s4;
  res.s5=a.s5^b.s5;
  res.s6=a.s6^b.s6;
  res.s7=a.s7^b.s7;
  res.s8=a.s8^b.s8;
  res.s9=a.s9^b.s9;
  res.s10=a.s10^b.s10;
  res.s11=a.s11^b.s11;
  res.s12=a.s12^b.s12;
  res.s13=a.s13^b.s13;
  res.s14=a.s14^b.s14;
  res.s15=a.s15^b.s15;
  res.s16=a.s16^b.s16;
  return res;
}


inline static batch B_FFN_ALL_29(){
  batch res;
  res.s1=0x29;
  res.s2=0x29;
  res.s3=0x29;
  res.s4=0x29;
  res.s5=0x29;
  res.s6=0x29;
  res.s7=0x29;
  res.s8=0x29;
  res.s9=0x29;
  res.s10=0x29;
  res.s11=0x29;
  res.s12=0x29;
  res.s13=0x29;
  res.s14=0x29;
  res.s15=0x29;
  res.s16=0x29;
  return res;
}
inline static batch B_FFN_ALL_02(){
  batch res;
  res.s1=0x02;
  res.s2=0x02;
  res.s3=0x02;
  res.s4=0x02;
  res.s5=0x02;
  res.s6=0x02;
  res.s7=0x02;
  res.s8=0x02;
  res.s9=0x02;
  res.s10=0x02;
  res.s11=0x02;
  res.s12=0x02;
  res.s13=0x02;
  res.s14=0x02;
  res.s15=0x02;
  res.s16=0x02;
  return res;
}
inline static batch B_FFN_ALL_04(){
  batch res;
  res.s1=0x04;
  res.s2=0x04;
  res.s3=0x04;
  res.s4=0x04;
  res.s5=0x04;
  res.s6=0x04;
  res.s7=0x04;
  res.s8=0x04;
  res.s9=0x04;
  res.s10=0x04;
  res.s11=0x04;
  res.s12=0x04;
  res.s13=0x04;
  res.s14=0x04;
  res.s15=0x04;
  res.s16=0x04;
  return res;
}
inline static batch B_FFN_ALL_10(){
  batch res;
  res.s1=0x10;
  res.s2=0x10;
  res.s3=0x10;
  res.s4=0x10;
  res.s5=0x10;
  res.s6=0x10;
  res.s7=0x10;
  res.s8=0x10;
  res.s9=0x10;
  res.s10=0x10;
  res.s11=0x10;
  res.s12=0x10;
  res.s13=0x10;
  res.s14=0x10;
  res.s15=0x10;
  res.s16=0x10;
  return res;
}
inline static batch B_FFN_ALL_40(){
  batch res;
  res.s1=0x40;
  res.s2=0x40;
  res.s3=0x40;
  res.s4=0x40;
  res.s5=0x40;
  res.s6=0x40;
  res.s7=0x40;
  res.s8=0x40;
  res.s9=0x40;
  res.s10=0x40;
  res.s11=0x40;
  res.s12=0x40;
  res.s13=0x40;
  res.s14=0x40;
  res.s15=0x40;
  res.s16=0x40;
  return res;
}
inline static batch B_FFN_ALL_80(){
  batch res;
  res.s1=0x80;
  res.s2=0x80;
  res.s3=0x80;
  res.s4=0x80;
  res.s5=0x80;
  res.s6=0x80;
  res.s7=0x80;
  res.s8=0x80;
  res.s9=0x80;
  res.s10=0x80;
  res.s11=0x80;
  res.s12=0x80;
  res.s13=0x80;
  res.s14=0x80;
  res.s15=0x80;
  res.s16=0x80;
  return res;
}

inline static batch B_FFSH8L(batch a,int n){
  batch res;
  res.s1=a.s1<<n;
  res.s2=a.s2<<n;
  res.s3=a.s3<<n;
  res.s4=a.s4<<n;
  res.s5=a.s5<<n;
  res.s6=a.s6<<n;
  res.s7=a.s7<<n;
  res.s8=a.s8<<n;
  res.s9=a.s9<<n;
  res.s10=a.s10<<n;
  res.s11=a.s11<<n;
  res.s12=a.s12<<n;
  res.s13=a.s13<<n;
  res.s14=a.s14<<n;
  res.s15=a.s15<<n;
  res.s16=a.s16<<n;
  return res;
}

inline static batch B_FFSH8R(batch a,int n){
  batch res;
  res.s1=a.s1>>n;
  res.s2=a.s2>>n;
  res.s3=a.s3>>n;
  res.s4=a.s4>>n;
  res.s5=a.s5>>n;
  res.s6=a.s6>>n;
  res.s7=a.s7>>n;
  res.s8=a.s8>>n;
  res.s9=a.s9>>n;
  res.s10=a.s10>>n;
  res.s11=a.s11>>n;
  res.s12=a.s12>>n;
  res.s13=a.s13>>n;
  res.s14=a.s14>>n;
  res.s15=a.s15>>n;
  res.s16=a.s16>>n;
  return res;
}


inline static void M_EMPTY(void){
}
