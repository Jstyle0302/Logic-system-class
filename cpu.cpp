#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

int convert2to10(bool num2[8]);
void convert_pc(bool pc[8],int num);
bool nor(bool a,bool b);

void mov(bool op[8],bool rd[4][8]);                             //mov原型
void add(bool sel,bool op[8],bool rd[4][8]);                    //add原型
void sub(bool sel,bool op[8],bool rd[4][8]);                    //sub原型
void ld(bool sel,bool op[8],bool rd[4][8],bool mem[256][8]);    //ld原型
void st(bool sel,bool op[8],bool rd[4][8],bool mem[256][8]);    //st原型
void bra(bool op[8],int &pc);                                   //bra原型


int main()
{
    bool data[256][8]={0};              //data memory
	bool operation[256][8]={0};         //instruction
	bool reg[4][8];                     //register
	string str;
	int  pc=0;                          //program counter integer
    bool program_counter[8]={0};        //program counter 8-bit

    ifstream fin("input.txt",ios::in);
    if(fin.fail())                      //輸入錯誤處理
	{
		cout<<"Input file opening failed.\n";
		exit(1);
	}
	while(fin>>str){
        for(int i=0;i<8;i++){
            operation[pc][i]=str[i]-'0';
        }
        pc++;
	}

	for(pc=0;pc<256;pc++){
        if(operation[pc][0]==0 && operation[pc][1]==0 &&operation[pc][2]==0){break;}
        else if(operation[pc][0]==0 && operation[pc][1]==0 &&operation[pc][2]==1){ mov(operation[pc],reg);}
        else if(operation[pc][0]==0 && operation[pc][1]==1 &&operation[pc][2]==0){ add(operation[pc][3],operation[pc],reg);}
        else if(operation[pc][0]==0 && operation[pc][1]==1 &&operation[pc][2]==1){ sub(operation[pc][3],operation[pc],reg);}
        else if(operation[pc][0]==1 && operation[pc][1]==0 &&operation[pc][2]==0){ ld(operation[pc][3],operation[pc],reg,data);}
        else if(operation[pc][0]==1 && operation[pc][1]==0 &&operation[pc][2]==1){ st(operation[pc][3],operation[pc],reg,data);}
        else if(operation[pc][0]==1 && operation[pc][1]==1 &&operation[pc][2]==0){ bra(operation[pc],pc);
        pc--;}              //使pc的值不會因為for迴圈跑完而在多加1
        else {break;}

	}

	ofstream fout("output.txt",ios::out);
	if(fout.fail())                             //輸出錯誤處理
	{
		cout<<"Output file opening failed.\n";
		exit(1);
	}

	for (int i=0;i<256;i++)                     //輸出data memory
	{
		fout<<"MEM["<<i<<"] = "<<data[i][0]<<data[i][1]<<data[i][2]<<data[i][3]<<data[i][4]<<data[i][5]<<data[i][6]<<data[i][7]<<endl;
	}
	fout<<endl;
	for (int i=0;i<4;i++)                       //輸出register
	{
		fout<<"REG["<<i<<"] = "<<reg[i][0]<<reg[i][1]<<reg[i][2]<<reg[i][3]<<reg[i][4]<<reg[i][5]<<reg[i][6]<<reg[i][7]<<endl;
	}
	fout<<endl;

    convert_pc(program_counter,pc);             //轉換pc成8-bits
	fout<<"current PC="<<program_counter[0]<<program_counter[1]<<program_counter[2]<<program_counter[3]<<program_counter[4]<<program_counter[5]<<program_counter[6]<<program_counter[7];
    return 0;
}

int convert2to10(bool num2[8]){                 //二進制轉十進制
    int num10=0;
    for(int i=0;i<8;i++){
            num10=num10*2+num2[i];
    }
    return num10;

}

void convert_pc(bool pc[8],int num){            //轉換pc型態
    for(int i=7;i>=0;i--){
        pc[i]=num%2;
        num=(num-(num%2))/2;
    }
}

bool nor(bool a,bool b){                        //nor運算
    return (!a)*b+(!b)*a;
}

void mov(bool op[8],bool rd[4][8]){             //001 mov指令
    bool destiny[8]={0,0,0,0,0,0,op[3],op[4]};
    bool imm[8]={0,0,0,0,0,op[5],op[6],op[7]};
    int rdd=convert2to10(destiny);
    for(int i=0;i<=7;i++){
        rd[rdd][i]=imm[i];
    }
}

void add(bool sel,bool op[8],bool rd[4][8]){    //010 add指令
    bool destiny[8]={0,0,0,0,0,0,op[4],op[5]};
    int rdd=convert2to10(destiny);
    bool carry=0;

    if(sel==0){
        bool rs[8]={0,0,0,0,0,0,op[6],op[7]};
        int rss=convert2to10(rs);
        bool tmp=0;
        for(int i=7;i>=0;i--){
            tmp=((rd[rdd][i]&&rd[rss][i]&&carry)||(rd[rdd][i]&&!rd[rss][i]&&!carry)||(!rd[rdd][i]&&rd[rss][i]&&!carry)||(!rd[rdd][i]&&!rd[rss][i]&&carry));
            carry= ((carry && rd[rss][i])||(rd[rss][i]&&rd[rdd][i])||(rd[rdd][i]&&carry));
            rd[rdd][i]=tmp;
        }
    }
    else{
        bool imm[8]={0,0,0,0,0,0,op[6],op[7]};
        bool tmp=0;
        for(int i=7;i>=0;i--){
            tmp=nor(carry,nor(imm[i],rd[rdd][i]));
            carry= ((carry && imm[i])||(imm[i]&&rd[rdd][i])||(rd[rdd][i]&&carry));
            rd[rdd][i]=tmp;
        }
    }
}

void sub(bool sel,bool op[8],bool rd[4][8]){    //011 sub指令
    bool destiny[8]={0,0,0,0,0,0,op[4],op[5]};
    int rdd=convert2to10(destiny);
    bool carry=0;

    if(sel==0){
        bool rs[8]={0,0,0,0,0,0,op[6],op[7]};
        int rss=convert2to10(rs);
        bool tmp=0;
        for(int i=7;i>=0;i--){
            tmp=((rd[rdd][i]&&!rd[rss][i]&&!carry)||(!rd[rdd][i]&&rd[rss][i]&&!carry)||(!rd[rdd][i]&&!rd[rss][i]&&carry)||(rd[rdd][i]&&rd[rss][i]&&carry));;
            carry= ((carry && (rd[rdd][i]==0))||(rd[rss][i]&&(rd[rdd][i]==0))||(rd[rss][i]&&carry));
            rd[rdd][i]=tmp;
        }
    }
    else{
        bool imm[8]={0,0,0,0,0,0,op[6],op[7]};
        bool tmp=0;
        for(int i=7;i>=0;i--){
            tmp=((rd[rdd][i]&&!imm[i]&&!carry)||(!rd[rdd][i]&&imm[i]&&!carry)||(!rd[rdd][i]&&!imm[i]&&carry)||(rd[rdd][i]&&imm[i]&&carry));;
            carry= ((carry && (rd[rdd][i]==0))||(imm[i]&&(rd[rdd][i]==0))||(imm[i]&&carry));
            rd[rdd][i]=tmp;
        }
    }
}

void ld(bool sel,bool op[8],bool rd[4][8],bool mem[256][8]){    //100 load指令
    bool destiny[8]={0,0,0,0,0,0,op[4],op[5]};
    int rdd=convert2to10(destiny);

    if(sel==0){
        bool rs[8]={0,0,0,0,0,0,op[6],op[7]};
        int rss=convert2to10(rs);
        bool rsss[8];
        for(int i=0;i<=7;i++){
            rsss[i]=rd[rss][i];
        }
        int Rss=convert2to10(rsss);
        for (int i=0;i<=7;i++){
            rd[rdd][i]=mem[Rss][i];
        }
    }
    else{
        bool adder[8]={0,0,0,0,0,0,op[6],op[7]};
        int addr=convert2to10(adder);
        for(int i=0;i<=7;i++){
            rd[rdd][i]=mem[addr][i];
        }
    }
}

void st(bool sel,bool op[8],bool rd[4][8],bool mem[256][8]){    //101 store指令
    bool destiny[8]={0,0,0,0,0,0,op[4],op[5]};
    int rdd=convert2to10(destiny);

    if(sel==0){
        bool rs[8]={0,0,0,0,0,0,op[6],op[7]};
        int rss=convert2to10(rs);
        bool rsss[8];
        for(int i=0;i<=7;i++){
            rsss[i]=rd[rss][i];
        }
        int Rss=convert2to10(rsss);
        for (int i=0;i<=7;i++){
            mem[Rss][i]=rd[rdd][i];
        }
    }
    else{
        bool adder[8]={0,0,0,0,0,0,op[6],op[7]};
        int addr=convert2to10(adder);
        for(int i=0;i<=7;i++){
            mem[addr][i]=rd[rdd][i];
        }
    }
}

void bra(bool op[8],int &pc){                                   //110 branch指令
    int offset=0;
    if(op[3]==0){
        bool off[8]={0,0,0,op[3],op[4],op[5],op[6],op[7]};
        offset =convert2to10(off);
        pc=pc+offset;
    }
    else{
        bool off[8]={0,0,0,!op[3],!op[4],!op[5],!op[6],!op[7]};
        offset =-(convert2to10(off)+1);
        pc=pc+offset;
    }

}


