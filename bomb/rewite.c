#include<stdio.h>

int phase_2(){
	rsi=rsp;
	if((rsp)==1){
		rbx=rsp+0x4;
		rbp=rsp+0x18;
		
		eax=rbx-0x4;
		eax*=2;
		if((rbx)==eax){
			rbx=rbx+0x4;
			if(rbx!=rbp)
				goto line9;
			else
				return ;
		}
		else boom!
	}
	else 
		boom!
} 



//x=edx=14,y=esi=0,z=ecx,val=eax,k=edi 
int func4(int x,int y,int k){
	val=x;
	val=val-y;
	z=val;
	z=z>>31;
	val=val+z;
	val=val>>1;
	z=val+y;
	if(z>k){
		x=z-1;
		func4();
		val=val*2;
		return val;
	}
	else{
		val=0;
		if(z<k){
			y=z+1;
			func4();
			val=val*2+1;
		}
		else return val;
	}
}

int phase_5(){
	char *A;
	A= 0x4024b0;
	string str;
	cin>>str;
	eax=srt.length();
	if(eax!=6)
		bomb!
	else{
		eax=0;
	L1:
		ecx=rbx+rax*1;
		(rsp)=cl;
		rdx=(rsp);
		edx=edx&1111;
		edx=A[rdx];
		rsp+rax*1+0x10=dl;
		rax++;
		
		if(eax==0x6){
			(rsp+0x16)=0;
			esi="flyers";
			rdi=0x10+rsp;
			if(eax==0){
				rax=rsp+0x18;
			}		
			else
				boom!
		}
		else
			goto L1;
			
		
	}
}

int phase_6(){
	r13=rsp;
	rsi=rsp;
	//读入六个数，这六个数在(rsp)~(rsp+0x18)
	r14=rsp;
	r12d=0; 
	rbp=r13;
	eax=(r13);  //eax为第一个输入的值
	eax-=0x1;   //这句话什么意思
	if(eax<=0x5){
		r12d+=0x1;
		if(r12d!=0x6){  //循环的判断条件
			ebx=r12d;   //第一轮ebx=1
			rax=ebx;    //rax=r12d=1
			eax=rsp+rax*4;  //eax=num[i] rax=i 第一轮eax=1
			if(eax!=(rbp)){ //num[0]!=num[1]
				ebx+=0x1;   //ebx=2
				if(ebx>0x5){    
					r13+=0x4;
					goto line94:rbp=r13;
				}
				else{       //ebx<=5 
					goto line101:rax=ebx;
				}
			}
			else boom!
		}
		else{
			rsi=(rsp+0x18);     //指向&num[6]+4
			rax=r14;            //rax=r14=rsp
			ecx=0x7;                 
			edx=ecx;
			edx-=(rax);         //
			(rax)=edx;
			rax+=0x4;
			if(rax==rsi){
				esi=0;
				ecx=rsp+rsi*1;
				if(ecx>0x1){
					eax=0x1;
					edx=0x6032d0;
					rdx=(rdx+0x8);
					eax+=0x1;
					if(eax==ecx){
						rdx=0x20+rsp+rsi*2;
						rsi+=0x4;
						if(rsi==0x18)
							goto line149:rbx=(rsp+0x20);
						else
							goto line125:ecx=rsp+rsi*1;
					}
					else{
						goto 
					}
				}
				else{
					edx=0x6032d0;
					rdx=0x20+rsp+rsi*2;
					rsi+=0x4;
					if(rsi!=0x18){
						goto line125:ecx=rsp+rsi*1;
					}
					else{
						rbx=(rsp+0x20);
						rax=(rsp+0x28);
						rsi=(rsp+0x50);
						rcx=rbx;
						rdx=(rax);    //rax里面存的是地址而不是普通数据了;
						(rcx+0x8)=rdx;
						rax+=0x8;
						if(rax==rsi){
							rcx=rdx;
							goto line141:rdx=(rax);
						}
						else{
							(rdx+8)=0;
							ebp=0x5;
							rax=(rbx+0x8);
							eax=(rax);
							if((rbx)>=eax){
								rbx=(rbx+0x8);
								ebp-=0x1;
								if(ebp!=0)
									goto line151:rax=(rbx+0x8);
							}
							else 
								bomb!
						}
					}
				}
			}
			else{
				ecx=edx;
				edx-=(rax);
				(rax)=edx;
				rax+=0x4;
				goto line123;
			}
		}
	}
	else 
		boom!
	
}