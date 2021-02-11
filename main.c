#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int isnum(char a)
{
    if(a>='0'&&a<='9')return 1;
    else if(a=='.')return 1;
    else return 0;
}
int isempty(char a[])
{
    if(a[0]=='\0')return 1;
    else return 0;
}
int isoperator(char a)
{
    if(a=='+'||a=='-'||a=='*'||a=='/'||a=='^'||a=='('||a==')')return 1;
    else return 0;
}
int olevel(char a)
{
    switch(a)
    {
        case '+':return 1;break;
        case '-':return 1;break;
        case '*':return 2;break;
        case '/':return 2;break;
        case '^':return 3;break;
            //case '(':return 20;break;
        default: return -1;break;
    }
}
void armove(char a[], int length, char elm, int tar)//add elm into a[] on tar
{
    for(int i=length; i>tar; i--)
    {
        a[i]=a[i-1];

    }
    a[tar]=elm;
}
void addsub(char a[], char sub[], int length, int tar)//add substr into a[] on tar
{
    int sublength=strlen(sub);//printf("sublength=%d\n",sublength);
    for(int i=length; i-sublength+1>tar; i--)
    {
        a[i]=a[i-sublength];
    }
    //puts(a);
    for (int i=0;i<sublength;i++)
    {
        a[tar+i]=sub[i];
    }
}
void lcancel(char a[],int length, int tar)//cut a[tar] and elm before it
{
    for(int i=0;i<length-tar-1;i++)
    {
        a[i]=a[i+tar+1];
    }
    if(length-tar-1<length)
        a[length-tar-1]='\0';
}
void lcmove(char a[],int length, int ta,int tb)//cut elm between ta and tb include ta and tb
{
    for(int i=ta;i<length-(tb-ta)-2;i++)
    {
        a[i]=a[i+tb-ta+1];
    }
    for(int i=length-(tb-ta)-2;i<length;i++)
    {
        a[i]='\0';
    }
}
int substr(char dst[],char src[],int start,int len) {
    char *p = src + start;
    int n = strlen(p);
    int i = 0;
    if (n < len) {
        len = n;
    }
    while (len != 0) {
        dst[i] = src[i + start];
        len--;
        i++;
    }
    dst[i] = '\0';
    return 0;
}
int main() {
    char LineCommand[150]={0};//get the command of every line
    FILE* fp=fopen("command.txt","r");
    struct
    {
        double data[150];
        int state[150];//-1 means empty, 0 means num, 1 means operator (char)
        int top;
    }IPExpression, stack;

    char VarName[20][20]={{0}};
    double VarVal[20]={0};
    int VarNum=0;
    int ChangeNum=-1;//about which variable to change. -1 means no
    int OutputMode=1;//about how to output, 0 means with ';' and no output, 2 means with fprintf
    while (!feof(fp))
    {
        for (int i = 0; i < 150; ++i)
        {
            LineCommand[i]='\0';
        }
        fgets(LineCommand,145,fp);
        if (LineCommand[0]=='\0'||LineCommand[0]=='\n')//omit empty line
            continue;
        //puts(LineCommand);
        /*
        for (int i = 0; i < strlen(LineCommand); ++i)
        {
            printf("%c-",LineCommand[i]);
        }
         */
        for (int i=0;i<145;i++)
        {
            if(LineCommand[i]=='\n')
                LineCommand[i]='\0';
        }
        ChangeNum=-1;
        OutputMode=1;
        IPExpression.top=0;
        stack.top=0;
        for(int i=0;i<150;i++)
        {
            IPExpression.state[i]=-1; //init IPE and stack
            stack.state[i]=-1;
        }

        char DIR[120]={0};
        //preprocess, get the algebra expression
        int ValueMode=0;//0 means normal algebraic expression, 1 means assignment statement, 2 means fprintf
        if(strstr(LineCommand,"fprintf"))
            ValueMode=2;
        else if (strstr(LineCommand,"="))
            ValueMode=1;
        if (ValueMode==1)
        {
            int equal_place=0;
            int start_place=0;
            char tempname[20]={0};
            int CreateMode=1;
            //puts(LineCommand);
            for (int i = 0; i < strlen(LineCommand); ++i)
            {
                if (LineCommand[i]!=' '){start_place=i;break;}
            }
            for (int i = 0; i < strlen(LineCommand); ++i)
            {
                if (LineCommand[i]=='='){equal_place=i;break;}
            }
            //get the name of the variable

            substr(tempname,LineCommand,start_place,equal_place-start_place);
            for (int i = 0; i < strlen(tempname); ++i)
            {
                if(tempname[i]==' ')
                    tempname[i]='\0';
            }
            //puts(tempname);

            //upload
            for (int i = 0; i < VarNum; ++i)
            {
                if(strcmp(VarName[i],tempname)==0)
                {
                    ChangeNum=i;
                    CreateMode=0;
                }
            }
            if(CreateMode) //new variable
            {
                ChangeNum=VarNum;
                strcpy(VarName[ChangeNum],tempname);
                ++VarNum;
            }

            //cut the part to get pure algebraic expression

            lcancel(LineCommand,strlen(LineCommand),equal_place);
        }
        else if (ValueMode==2)//find fprintf
        {
            OutputMode=2;
            int P1,P2;
            int L1=-1,L2;
            /*example:
             * fprintf( "a=%f "   ,  a  )
             *         P1     P2  L1    L2
             */
            for (int i = 0; i < 120; ++i)
            {
                if(LineCommand[i]=='\"')
                {
                    P1 = i+1;
                    break;
                }
            }
            for (int i = P1+1; i < 120; ++i)
            {
                if(LineCommand[i]=='\"')
                {
                    P2=i-1;
                    break;
                }
            }
            char DirPart[100]={0};//part inside " "
            substr(DirPart,LineCommand,P1,P2-P1+1);
            //change \n to '\n'
            for (int i=0;i<strlen(DirPart)-1;i++)
            {
                if (DirPart[i]=='\\'&&DirPart[i+1]=='n')
                {
                    DirPart[i]=' ';
                    DirPart[i+1]='\n';
                }
            }
            strcpy(DIR,DirPart);

            //find "," and ")"
            for (int i = P2+1; i < 130; ++i) //assume "," appear only once
            {
                if (LineCommand[i]==',')
                    L1=i;
                if (LineCommand[i]==')')
                    L2=i;
            }

            if(L1==-1)
            {
                LineCommand[0]='\0';
                printf("%s", DirPart);
                OutputMode=0;

            }
            else
            {
                char TempCommand[120]={0};
                substr(TempCommand,LineCommand,L1+1,L2-L1-1);
                strcpy(LineCommand,TempCommand);
            }

        }
        if (OutputMode==1)
            for (int i = 0; i < 130; ++i)
            {
                if (LineCommand[i]==';')
                {
                    OutputMode = 0;
                    LineCommand[i]=' ';
                }
            }
        //substitute variable in algebraic expression

        for (int i = 0; i < strlen(LineCommand); ++i)
        {
            if((LineCommand[i]>='A'&&LineCommand[i]<='Z')||(LineCommand[i]>='a'&&LineCommand[i]<='z')||LineCommand[i]=='_')//find variable
            {
                int VFlag1=i,VFlag2=i;
                while (!(isoperator(LineCommand[VFlag2+1])||LineCommand[VFlag2+1]=='\0'||LineCommand[VFlag2+1]==' '))
                {
                    VFlag2++;
                }
                char tochange[20]={0}; //wait to substitute
                substr(tochange,LineCommand,VFlag1,VFlag2-VFlag1+1);
                //printf("tochange=%s|\n",tochange);
                for (int j = 0; j < VarNum; ++j)
                {
                    if (strcmp(tochange,VarName[j])==0)//find
                    {

                        double value=VarVal[j];
                        char cvalue[20]={0};
                        gcvt(value,10,cvalue);
                        //printf("confirm:%lf-%s\n",value,cvalue);
                        lcmove(LineCommand,120,VFlag1,VFlag2);
                        //printf("after cut:%s\n",LineCommand);
                        addsub(LineCommand,cvalue,120,VFlag1);
                        //printf("after change:%s\n",LineCommand);

                    }
                }
            }
        }

        //deal with "-a" problem, change it to (0-a)
        for (int i = 0; i < strlen(LineCommand); ++i)
        {
            if(LineCommand[i]=='-')
            {
                int flag1=i;
                int flag2=i;
                int needdeal=0;
                while (1)
                {
                    if(flag1==0)
                    {
                        needdeal=1;
                        break;
                    }
                    --flag1;
                    if(isoperator(LineCommand[flag1]))
                    {
                        needdeal=1;
                        break;
                    }
                    else if (LineCommand[flag1]>='0'&&LineCommand[flag1]<='9')
                    {
                        needdeal=0;
                        break;
                    }
                }
                if(needdeal)
                {
                    //armove(LineCommand,120,'0',i);
                    addsub(LineCommand,"(0",120,i);
                    int pb=i+3;
                    while(isnum(LineCommand[pb])||LineCommand[pb]==' ')++pb;
                    armove(LineCommand,120,')',pb);
                    //printf("after=%s\n",LineCommand);
                }
            }
        }
        if (OutputMode==1)
        for (int i = 0; i < 130; ++i)
        {
            if (LineCommand[i]==';')
            {
                OutputMode = 0;
                LineCommand[i]=' ';
            }
        }

        //preprocess end, calculate begin!
        for(int i=0;i<strlen(LineCommand);i++)//turn algebra command into IPE
        {
            if(LineCommand[i]=='\0'||LineCommand[i]=='\n'){}
            else if(isnum(LineCommand[i]))//get a number
            {
                int pa=i,pb=i;
                char temp[2]={0}; //get single num character
                char cnum[15]={0}; //get the num;

                double num=0;
                while(isnum(LineCommand[pb]))
                {
                    temp[0]=LineCommand[pb];
                    strcat(cnum,temp);
                    pb++;
                }
                --pb;
                num=atof(cnum);
                IPExpression.data[IPExpression.top]=num;
                IPExpression.state[IPExpression.top]=0;
                ++IPExpression.top;
                i=pb;
            }
            else if (LineCommand[i]==' '){}
            else if(stack.top==0||stack.data[stack.top-1]=='(')
            {
                stack.data[stack.top++]=LineCommand[i];//push

            }
            else if(LineCommand[i]=='(')
            {
                stack.data[stack.top++]=LineCommand[i];//push

            }
            else if(LineCommand[i]==')')//pop and IPE until '(', and throw '('
            {
                while(stack.data[stack.top-1]!='(')
                {
                    char elm=stack.data[--stack.top];//pop
                    IPExpression.data[IPExpression.top]=elm;
                    IPExpression.state[IPExpression.top]=1;
                    ++IPExpression.top;//put into IPE
                }
                --stack.top;//throw '('
            }
            else if(olevel(LineCommand[i])>olevel(stack.data[stack.top-1]))
            {
                stack.data[stack.top++] = LineCommand[i];//push
            }
            else if(olevel(LineCommand[i])==olevel(stack.data[stack.top-1]))//pop and IPE, then push
            {
                char elm=stack.data[--stack.top];//pop
                IPExpression.data[IPExpression.top]=elm;
                IPExpression.state[IPExpression.top]=1;
                ++IPExpression.top;//put into IPE
                stack.data[stack.top++]=LineCommand[i];//push
            }
            else if(olevel(LineCommand[i])<olevel(stack.data[stack.top-1]))//pop and IPE, then compare again
            {
                label1:;
                char elm=stack.data[--stack.top];//pop
                IPExpression.data[IPExpression.top]=elm;
                IPExpression.state[IPExpression.top]=1;
                ++IPExpression.top;//put into IPE
                //redo
                if(olevel(LineCommand[i])>olevel(stack.data[stack.top-1]))
                {
                    stack.data[stack.top++] = LineCommand[i];//push
                }
                else if(olevel(LineCommand[i])==olevel(stack.data[stack.top-1]))//pop and IPE, then push
                {
                    char elm2=stack.data[--stack.top];//pop
                    IPExpression.data[IPExpression.top]=elm2;
                    IPExpression.state[IPExpression.top]=1;
                    ++IPExpression.top;//put into IPE
                    stack.data[stack.top++]=LineCommand[i];//push
                }
                else goto label1;

            }
            else printf("here? top=%d\n",stack.top);
        }
        //end, pop and IPE all
        while(stack.top!=0)
        {
            char elm=stack.data[--stack.top];//pop
            IPExpression.data[IPExpression.top]=elm;
            IPExpression.state[IPExpression.top]=1;
            ++IPExpression.top;//put into IPE
        }
        //successfully turn into IPE
        //calculate IPE
        for(int i=0;i<IPExpression.top;i++)
        {
            if (IPExpression.state[i]==0)
            {
                stack.data[stack.top++]=IPExpression.data[i];
            }
            else if(IPExpression.state[i]==1)
            {
                switch ((char)IPExpression.data[i])
                {
                    case '+':stack.data[stack.top-2]+=stack.data[stack.top-1];break;
                    case '-':stack.data[stack.top-2]-=stack.data[stack.top-1];break;
                    case '*':stack.data[stack.top-2]*=stack.data[stack.top-1];break;
                    case '/':stack.data[stack.top-2]/=stack.data[stack.top-1];break;
                    case '^':stack.data[stack.top-2]=pow(stack.data[stack.top-2],stack.data[stack.top-1]);break;
                }
                --stack.top;
            }
        }
        double result=stack.data[0];
        if(ChangeNum>=0)//upload value
            VarVal[ChangeNum]=result;

        if (OutputMode==1)
        {
            printf("%lf\n",result);
        }
        else if (OutputMode==2)
        {
            int ModeDF=1;
            for (int i = 0; i < strlen(DIR)-1; ++i)
            {
                if (DIR[i]=='%')
                {
                    if (DIR[i+1]=='f')
                        ModeDF=2;
                }
            }
            if (ModeDF==2)
                printf(DIR,result);
            else
                printf(DIR,(int)result);
        }
    }
    fclose(fp);

    printf("---var list---\n");
    for (int i = 0; i < VarNum; ++i) //variable table
    {
        printf("%s=%lf\n",VarName[i],VarVal[i]);
    }
    system("pause");
    return 0;
}
