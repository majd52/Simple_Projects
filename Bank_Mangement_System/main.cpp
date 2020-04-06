#include<ctime>
#include<windows.h>
#include<conio.h>
#include<mysql.h>
#include<iostream>
#include<sstream>
#include<cstdio>
#include<string>
using namespace std;
MYSQL *conn;
MYSQL_ROW row;
MYSQL_RES* res;
int qstate;
bool admin=0;
string id;
string username;
string query;

void db_connection();
string get_date();
void sign_up();
void login();
void account();
void welcome();
void withdraw();
void account_balance();
void add_money();
void trans_funds();
void show_logs();
void show_accounts();
int main()
{
    db_connection();


    cout<<"1-LOGIN\n2-SIGNUP\n\nchoice :";
    char c;
    c=getch();
    switch (c){
        case '1':
    login();
    break;
case '2':
        sign_up();
        break;
     }
    while (1)
    {
        welcome();
        char i;
        cout<<"Choice :";
        i=getch();
        switch (i)
        {
        case '1' :
            account_balance();
            break;
        case '2':
            add_money();
            break;
        case '3' :
            trans_funds();
            break;
        case '4':
            withdraw();
            break;
        case '5':
            show_logs();
            break;
        case '6':
            show_accounts();
            break;
        case '7':
            system("cls");
            main();
        default :
            cout<<"\n\nwrong input !\n";
            system("pause");
        }
    }
}
void db_connection(){

    conn =mysql_init(0);

    conn=mysql_real_connect(conn,"127.0.0.1","root","","bank",3306,NULL,0);
    if (conn)
    {
        cout<<"successfully connected to database in host :"<<endl;
        cout<<mysql_get_host_info(conn)<<endl;

    }
    else
        cout<<"failluer to lunch : "<<mysql_error(conn)<<endl;
    system("pause");
    system("cls");
}
void sign_up(){
    while(1)
    { system("cls");
        string user_name,password;
        cout<<"ENTER USERNAME : ";
        cin>>user_name;
        query="select user from users where user like \'"+user_name+"\';";
        qstate=mysql_query(conn,query.c_str());
        res=mysql_store_result(conn);
        row=mysql_fetch_row(res);
        if(row==NULL)
        {
            string lname,fname;
            mysql_free_result(res);
            system("cls");
            cout<<"ENTER PASSWORD : ";
            char p=getch();
            while (p!='\r')
        {
            password+=p;
            cout<<"*";
            p=getch();

        }
            system("cls");
            cout<<"YOUR FIRST NAME : ";
            cin>>fname;
            cout<<"\nYOUR LAST NAME : ";
            cin>>lname;

            query="insert into users values(null,\'"+user_name+"\',\'"+password+"\',\'"+fname+"\',\'"+lname+"\',\'"+get_date()+"\');";
            qstate=mysql_query(conn,query.c_str());
            if(qstate){
                cout<<"\n\nWRONG INPUT TRY AGAIN ! "<<endl;
                system("pause");
            }
            username=user_name;
            query="select user,id from users where user like \'"+user_name+"\';";
            qstate=mysql_query(conn,query.c_str());
            res=mysql_store_result(conn);
            row=mysql_fetch_row(res);
            id=row[1];
            mysql_free_result(res);
            cout<<"\OK\n";
            system("pause");
            break;
        }
        else
            cout<<"\nthis username is not avalibale !\n";
        system("pause");
    }
    query="insert into account values("+id+",0);";
   mysql_query(conn,query.c_str());

}
string get_date(){
 time_t now=time(0);
    tm *itm =localtime(&now);

            stringstream ss; //for register time
  ss<<(itm->tm_year+1900);
   string regdate=ss.str();
   ss.str("");
   ss<<itm->tm_mon+1;
   regdate=regdate+"-"+ss.str();
   ss.str("");
   ss<<itm->tm_mday;
   regdate=regdate+"-"+ss.str();
   ss.str("");
   ss<<itm->tm_hour;
   regdate=regdate+" "+ss.str();
   ss.str("");
   ss<<itm->tm_min;
   regdate=regdate+":"+ss.str();
   ss.str("");
   ss<<itm->tm_sec;
   regdate=regdate+":"+ss.str();
   ss.str("");
   return regdate;
}
void login(){
    while(1)
    {
        system("cls");
        string password;
        cout<<"USERNAME:";
        cin>>username;
        cout<<"PASSWORD:";
        char p=getch();
        while (p!='\r')
        {
            password+=p;
            cout<<"*";
            p=getch();
        }
        query="SELECT user,pass,id FROM users where user like \'"+username+"\' and pass like \'"+password+"\';";
        system("cls");
        qstate=mysql_query(conn,query.c_str());
        if (!qstate)
        {
            res=mysql_use_result(conn);
            row=mysql_fetch_row(res);
            if (row==NULL)
            {
                cout<<"USER OR PASSWORD IS WRONG!"<<endl;
                system("pause");
            }
            else
            {
                id=row[2];
                mysql_free_result(res);
                break;
            }
        }
    }
}
void welcome(){
    system("cls");

         cout<<"Welcome to Bank Mangement Account "<<endl;
    account();
    cout<<"Menu :"<<endl;
    cout<<"01-Account Balance"<<endl;
    cout<<"02-Add Funds To Your Balance"<<endl;
    cout<<"03-Transfer Funds To Another Account"<<endl;
    cout<<"04-Withdraw Fund "<<endl;
    cout<<"05-Logs"<<endl;
    cout<<"06-Show All Accounts Funds"<<endl;
    cout<<"07-Sign Out"<<endl;
}
void account(){

    cout<<"Username : "<<username<<"\n"<<"id : "<<id<<endl<<endl;
}
void account_balance(){
    system("cls");
    account();
    query="select balance from account where uid="+id+";";
    mysql_query(conn,query.c_str());
    res=mysql_store_result(conn);
    row=mysql_fetch_row(res);
    cout<<"your current balance : "<<row[0]<<" $"<<endl;
    system("pause");
}
void add_money(){
system("cls");
string money;
cout<<"Enter the amount of amount of money you inserted : ";
cin>>money;
mysql_autocommit(conn,0);
mysql_query(conn,"begin;");
query="insert into logs values("+id+",\'"+get_date()+"\',\'Add Money To Your Funds With Amount : \',"+money+");";
mysql_query(conn,query.c_str());
query="update account set balance=balance+"+money+" where uid="+id+";";
qstate=mysql_query(conn,query.c_str());
mysql_query(conn,"commit;");
if(!qstate)
    cout<<"\n\nDONE\n\n";
    else
    cout<<"\n\nWRONG INPUT\n\n";
system("pause");

}
void trans_funds(){
system("cls");
string dusername,did;
long int dmoney;
cout<<"Enter The Username Of The Account You Want To Transfer Funds To : ";
cin>>dusername;
if (dusername==username){
    cout<<"you add money to your account dumb !\n\n";
    system("pause");
    return ;
}
query="select id from users where user=\'"+dusername+"\';";
qstate=mysql_query(conn,query.c_str());
res=mysql_store_result(conn);
row=mysql_fetch_row(res);
if(row==NULL){
    cout<<"\n\nthere is no username with name ! \n\n";
    mysql_free_result(res);
    system("pause");
    return ;

}
else{
        did=row[0];
        mysql_free_result(res);
cout<<"\n\nEnter The Amount Of Funds You Want To Transfer : ";
cin>>dmoney;
query="select balance from account where uid="+id+";";
qstate=mysql_query(conn,query.c_str());
res=mysql_store_result(conn);
row=mysql_fetch_row(res);

stringstream dd(row[0]);
int money=0;
dd>>money;
if (money<dmoney){

    cout<<"\nYou dont have this money dumb !!!\n";
    system("pause");
    return;
}
stringstream ss;
ss<<dmoney;
string ddmoney;
ddmoney=ss.str();

mysql_autocommit(conn,0);
mysql_query(conn,"begin;");
query=" update account set balance=balance-"+ddmoney+" where uid="+id+" ;  ";
qstate=mysql_query(conn,query.c_str());
query="insert into logs values("+id+",\'"+get_date()+"\',\'Transfer Funds To "+dusername+" With Amount : \',"+ddmoney+");";
qstate=mysql_query(conn,query.c_str());
query="update account set balance=balance+"+ddmoney+" where uid="+did+";";
qstate=mysql_query(conn,query.c_str());
query="insert into logs values("+did+",\'"+get_date()+"\',\'Recive Funds From "+username+" With Amount : \',"+ddmoney+");";
qstate=mysql_query(conn,query.c_str());
mysql_query(conn,"commit;");
mysql_autocommit(conn,1);

system("cls");
if(!qstate)
cout<<"\nYou Transfer Funds : "<<dmoney<<" To Account "<<dusername<<" Successfully !\n\n";
else{
    cout<<"\nWRONG INPUT!\n\n";
   }

system("pause");

}
}
void withdraw(){
system("cls");
cout <<"Enter the amount of money you want to withdraw : ";
int dmoney;
cin>>dmoney;
query="select balance from account where uid="+id+";";
mysql_query(conn,query.c_str());
res=mysql_store_result(conn);
row=mysql_fetch_row(res);
stringstream ss(row[0]);
int money=0;
ss>>money;
if(dmoney>money){
    cout<<"\n\nyou dont have this amount of money\n";
    system("pause");
return ;

}
else {
        stringstream dd;
dd<<dmoney;
mysql_autocommit(conn,0);
mysql_query(conn,"begin;");
    query="update account set balance=balance-"+dd.str()+"where uid="+id+";";

mysql_query(conn,query.c_str());
query="insert into logs values("+id+",\'"+get_date()+"\',\'Withdraw funds From Your Account With Amount : \',"+dd.str()+");";
mysql_query(conn,query.c_str());
mysql_query(conn,"commit;");
cout<<"\n\nMoney withdrawn successfully\n\n ";
system("pause");
}


}
void show_logs(){
system("cls");
        printf("----------------------------------------------------------------------------------\n");
        printf("| %-55s | %-21s |\n", "Title", "DATE");
        printf("----------------------------------------------------------------------------------\n");
        query="select reg_date from users where id="+id+";";
        qstate=mysql_query(conn,query.c_str());
        res=mysql_store_result(conn);
        row=mysql_fetch_row(res);
        printf("| %-55s | %-21s |\n", "Create Account in This Bank ", row[0]);

        mysql_free_result(res);
        query="select title,amount,l_date from logs where uid="+id+" order by l_date desc;";
qstate=mysql_query(conn,query.c_str());
if(!qstate){
res=mysql_store_result(conn);

while((row=mysql_fetch_row(res))){
        char *temp=row[0];
        string title=temp;
        temp=row[1];
        title=title+temp+" $"   ;
char *ttitle=const_cast<char*>(title.c_str());
printf("| %-55s | %-21s |\n", ttitle, row[2]);

}

mysql_free_result(res);
printf("----------------------------------------------------------------------------------\n\n");
while (1){
cout<<"C clear logs , M main menu\nchoice : ";

char c=getch();
switch(c){
case 'm' :
case 'M':
    return ;
case 'C':
    case 'c' :
    query="delete from logs where uid="+id+";";
    mysql_query(conn,query.c_str());
    cout<<"\nlogs clear successfully \n";
    system("pause");
    return ;
    default :
        cout<<"  wrong input \n";
        system("pause");
} }
system("pause");
} }

void show_accounts(){
system("cls");
mysql_query(conn,"select u.id,u.user,u.fname,u.lname,a.balance,u.reg_date from users as u join account as a on u.id=a.uid;");

res=mysql_store_result(conn);
     printf("-------------------------------------------------------------------------------------------------------------\n");
     printf("| %-5s | %-15s | %-15s | %-15s | %-20s | %-21s |\n", "ID", "USER","FIRST NAME","LAST NAME","FUNDS","REG DATE");
     printf("-------------------------------------------------------------------------------------------------------------\n");

while (row=mysql_fetch_row(res)){
     printf("| %-5s | %-15s | %-15s | %-15s | %-20s | %-21s |\n", row[0], row[1],row[2],row[3],row[4],row[5]);
}
 printf("-------------------------------------------------------------------------------------------------------------\n");
cout<<endl<<endl;
system("pause"); }
