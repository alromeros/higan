void g65816_flags_lda_b(void) {
  g65816_testn(gx816->regs.a.b & 0x80);
  g65816_testz(gx816->regs.a.b == 0);
}

void g65816_flags_lda_w(void) {
  g65816_testn(gx816->regs.a.w & 0x8000);
  g65816_testz(gx816->regs.a.w == 0);
}

/************************
 *** 0xa9: lda #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/

void g65816_op_lda_constb(void) {
  gx816->regs.a.b = gx816->read_operand(1); //1,2 [op fetch]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_constw(void) {
  gx816->regs.a.w = gx816->read_operand(2); //1,2,2a [op fetch]

  g65816_flags_lda_w();
  g65816_incpc(3);
}

/**********************
 *** 0xad: lda addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/

void g65816_op_lda_addrb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                      //1-3 [op fetch]
  gx816->regs.a.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w); //4 [read]

  g65816_flags_lda_b();
  g65816_incpc(3);
}

void g65816_op_lda_addrw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                            //1-3 [op fetch]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //4 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //4a [read high]

  g65816_flags_lda_w();
  g65816_incpc(3);
}

/************************
 *** 0xbd: lda addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/

void g65816_op_lda_addrxb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                      //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.x);            //3a [pbc or p.x=0]
  gx816->regs.a.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x); //4 [read]

  g65816_flags_lda_b();
  g65816_incpc(3);
}

void g65816_op_lda_addrxw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                            //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.x);                  //3a [pbc or p.x=0]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x);     //4 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.x + 1); //4a [read high]

  g65816_flags_lda_w();
  g65816_incpc(3);
}

/********************
 *** 0xa5: lda dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/

void g65816_op_lda_dpb(void) {
  gx816->op.dp = gx816->read_operand(1);                     //1,2 [op fetch]
  gx816->op_cond(2);                                         //2a [dl!=0]
  gx816->regs.a.b = gx816->op_read(OPMODE_DP, gx816->op.dp); //3 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_dpw(void) {
  gx816->op.dp = gx816->read_operand(1);                           //1,2 [op fetch]
  gx816->op_cond(2);                                               //2a [dl!=0]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //3a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/**********************
 *** 0xb2: lda (dp) ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] dbr,aa   ; data low
  [5a] dbr,aa+1 ; data high [1]
*/

void g65816_op_lda_idpb(void) {
  gx816->op.dp = gx816->read_operand(1);                          //1,2 [op fetch]
  gx816->op_cond(2);                                              //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //4 [aah]
  gx816->regs.a.b  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);  //5 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_idpw(void) {
  gx816->op.dp = gx816->read_operand(1);                              //1,2 [op fetch]
  gx816->op_cond(2);                                                  //2a [dl!=0]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp);        //3 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);    //4 [aah]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);     //5 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1); //5a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/**********************
 *** 0xa7: lda [dp] ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] 0,d+dp+2 ; aab
  [6 ] aab,aa   ; data low
  [6a] aab,aa+1 ; data high [1]
*/

void g65816_op_lda_ildpb(void) {
  gx816->op.dp = gx816->read_operand(1);                          //1,2 [op fetch]
  gx816->op_cond(2);                                              //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);     //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1); //4 [aah]
  gx816->op.aa.p.b = gx816->op_read(OPMODE_DP, gx816->op.dp + 2); //5 [aab]
  gx816->regs.a.b  = gx816->op_read(OPMODE_LONG, gx816->op.aa.l); //6 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_ildpw(void) {
  gx816->op.dp = gx816->read_operand(1);                               //1,2 [op fetch]
  gx816->op_cond(2);                                                   //2a [dl!=0]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp);         //3 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);     //4 [aah]
  gx816->op.aa.p.b  = gx816->op_read(OPMODE_DP, gx816->op.dp + 2);     //5 [aab]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_LONG, gx816->op.aa.l);     //6 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + 1); //6a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/**********************
 *** 0xaf: lda long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/

void g65816_op_lda_longb(void) {
  gx816->op.aa.l = gx816->read_operand(3);                       //1-4 [op fetch]
  gx816->regs.a.b = gx816->op_read(OPMODE_LONG, gx816->op.aa.l); //5 [read]

  g65816_flags_lda_b();
  g65816_incpc(4);
}

void g65816_op_lda_longw(void) {
  gx816->op.aa.l = gx816->read_operand(3);                             //1-4 [op fetch]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_LONG, gx816->op.aa.l);     //5 [read]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + 1); //5a [read]

  g65816_flags_lda_w();
  g65816_incpc(4);
}

/************************
 *** 0xbf: lda long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/

void g65816_op_lda_longxb(void) {
  gx816->op.aa.l = gx816->read_operand(3);                                       //1-4 [op fetch]
  gx816->regs.a.b = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + gx816->regs.x); //5 [read]

  g65816_flags_lda_b();
  g65816_incpc(4);
}

void g65816_op_lda_longxw(void) {
  gx816->op.aa.l = gx816->read_operand(3);                                             //1-4 [op fetch]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + gx816->regs.x);     //5 [read]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + gx816->regs.x + 1); //5a [read]

  g65816_flags_lda_w();
  g65816_incpc(4);
}

/************************
 *** 0xb9: lda addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/

void g65816_op_lda_addryb(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                      //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.y);            //3a [pbc or p.x=0]
  gx816->regs.a.b = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y); //4 [read]

  g65816_flags_lda_b();
  g65816_incpc(3);
}

void g65816_op_lda_addryw(void) {
  gx816->op.aa.w = gx816->read_operand(2);                                            //1-3 [op fetch]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.y);                  //3a [pbc or p.x=0]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y);     //4 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1); //4a [read high]

  g65816_flags_lda_w();
  g65816_incpc(3);
}

/**********************
 *** 0xb5: lda dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/

void g65816_op_lda_dpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                     //1,2 [op fetch]
  gx816->op_cond(2);                                                         //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                             //3 [i/o]
  gx816->regs.a.b = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x); //4 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_dpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                           //1,2 [op fetch]
  gx816->op_cond(2);                                                               //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                   //3 [i/o]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //4a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/************************
 *** 0xa1: lda (dp,x) ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; aal
  [5 ] 0,d+dp+x+1 ; aah
  [6 ] dbr,aa     ; data low
  [6a] dbr,aa+1   ; data high [1]
*/

void g65816_op_lda_idpxb(void) {
  gx816->op.dp = gx816->read_operand(1);                                          //1,2 [op fetch]
  gx816->op_cond(2);                                                              //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                  //3 [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //5 [aah]
  gx816->regs.a.b  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);                  //6 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_idpxw(void) {
  gx816->op.dp = gx816->read_operand(1);                                           //1,2 [op fetch]
  gx816->op_cond(2);                                                               //2a [dl!=0]
  snes_time->add_cpu_icycles(1);                                                   //3 [i/o]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x);     //4 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + gx816->regs.x + 1); //5 [aah]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w);                  //6 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + 1);              //6 [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/************************
 *** 0xb1: lda (dp),y ***
 ************************
cycles:
  [1 ] pbr,pc         ; opcode
  [2 ] pbr,pc+1       ; dp
  [2a] pbr,pc+1       ; io [2]
  [3 ] 0,d+dp         ; aal
  [4 ] 0,d+dp+1       ; aah
  [4a] dbr,aah,aal+yl ; io [4]
  [5 ] dbr,aa+y       ; data low
  [5a] dbr,aa+y+1     ; data high [1]
*/

void g65816_op_lda_idpyb(void) {
  gx816->op.dp = gx816->read_operand(1);                                         //1,2 [op fetch]
  gx816->op_cond(2);                                                             //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);                    //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                //4 [aah]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.y);             //4a [pbc or p.x=0]
  gx816->regs.a.b  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y); //5 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_idpyw(void) {
  gx816->op.dp = gx816->read_operand(1);                                              //1,2 [op fetch]
  gx816->op_cond(2);                                                                  //2a [dl!=0]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp);                        //3 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                    //4 [aah]
  gx816->op_cond(4, gx816->op.aa.w, gx816->op.aa.w + gx816->regs.y);                  //4a [pbc or p.x=0]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y);     //5 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1); //5a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/************************
 *** 0xb7: lda [dp],y ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] 0,d+dp     ; aal
  [4 ] 0,d+dp+1   ; aah
  [5 ] 0,d+dp+2   ; aab
  [6 ] aab,aa+y   ; data low
  [6a] aab,aa+y+1 ; data high [1]
*/

void g65816_op_lda_ildpyb(void) {
  gx816->op.dp = gx816->read_operand(1);                                          //1,2 [op fetch]
  gx816->op_cond(2);                                                              //2a [dl!=0]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_DP, gx816->op.dp);                     //3 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                 //4 [aah]
  gx816->op.aa.p.b = gx816->op_read(OPMODE_DP, gx816->op.dp + 2);                 //5 [aab]
  gx816->regs.a.b  = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + gx816->regs.y); //6 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_ildpyw(void) {
  gx816->op.dp = gx816->read_operand(1);                                               //1,2 [op fetch]
  gx816->op_cond(2);                                                                   //2a [dl!=0]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_DP, gx816->op.dp);                         //3 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_DP, gx816->op.dp + 1);                     //4 [aah]
  gx816->op.aa.p.b  = gx816->op_read(OPMODE_DP, gx816->op.dp + 2);                     //5 [aab]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + gx816->regs.y);     //6 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_LONG, gx816->op.aa.l + gx816->regs.y + 1); //6a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/**********************
 *** 0xa3: lda sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/

void g65816_op_lda_srb(void) {
  gx816->op.sp = gx816->read_operand(1);                     //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                             //3 [i/o]
  gx816->regs.a.b = gx816->op_read(OPMODE_SP, gx816->op.sp); //4 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_srw(void) {
  gx816->op.sp = gx816->read_operand(1);                           //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                                   //3 [i/o]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_SP, gx816->op.sp);     //4 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_SP, gx816->op.sp + 1); //4a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}

/**************************
 *** 0xb3: lda (sr,s),y ***
 **************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; sp
  [3 ] pbr,pc+1   ; io
  [4 ] 0,s+sp     ; aal
  [5 ] 0,s+sp+1   ; aah
  [6 ] 0,s+sp+1   ; io
  [7 ] dbr,aa+y   ; data low
  [7a] dbr,aa+y+1 ; data high [1]
*/

void g65816_op_lda_isryb(void) {
  gx816->op.sp = gx816->read_operand(1);                                         //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                                                 //3 [i/o]
  gx816->op.aa.p.l = gx816->op_read(OPMODE_SP, gx816->op.sp);                    //4 [aal]
  gx816->op.aa.p.h = gx816->op_read(OPMODE_SP, gx816->op.sp + 1);                //5 [aah]
  snes_time->add_cpu_icycles(1);                                                 //6 [i/o]
  gx816->regs.a.b  = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y); //7 [read]

  g65816_flags_lda_b();
  g65816_incpc(2);
}

void g65816_op_lda_isryw(void) {
  gx816->op.sp = gx816->read_operand(1);                                              //1,2 [op fetch]
  snes_time->add_cpu_icycles(1);                                                      //3 [i/o]
  gx816->op.aa.p.l  = gx816->op_read(OPMODE_SP, gx816->op.sp);                        //4 [aal]
  gx816->op.aa.p.h  = gx816->op_read(OPMODE_SP, gx816->op.sp + 1);                    //5 [aah]
  snes_time->add_cpu_icycles(1);                                                      //6 [i/o]
  gx816->regs.a.p.l = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y);     //7 [read low]
  gx816->regs.a.p.h = gx816->op_read(OPMODE_DBR, gx816->op.aa.w + gx816->regs.y + 1); //7a [read high]

  g65816_flags_lda_w();
  g65816_incpc(2);
}
