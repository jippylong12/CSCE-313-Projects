//I haven't done anything but I believe the report says we need to write the script
//in this file. 
#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <iomanip>

using namespace std;

int main(int argc, char * argv[])
{
    string file_name="";
    string pid="";
    string request;
    do
    {
        cout<<"$  ";
        cin>>request;
        if(request=="help")
        {
            cout<<"COMMANDS\n";
            cout<<" pid\n view_ps \n Identifiers \n State \n Thread_Information \n Time_Information \n Address_Space \n Resources \n Processors \n Memory_map \n exit \n";
        }
        if(request.substr(0,3)=="pid")
        {
            cout<<"what is the pid:"<<endl;
            cin>>request;
            pid=request;
        }
        
        if(request=="view_ps")
        {
            pid_t all_ps=fork();
            
            if(all_ps==0)
            {
                char * arga[]={"ps", "a", NULL};
                execvp(arga[0],arga);
            }
            else
            {
                sleep(10);
            }
        }
        
        if(request=="Identifiers")
        {
            stringstream buffer;
            string line;
            file_name= "//proc//" +pid + "//status";
            
            ifstream file(file_name.c_str());
            
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                while(!file.eof())
                {
                    string a,b,c,d,e;
                    getline(file,line);
                    
                    if(line.substr(0,3)=="Pid")
                        cout<<line<<"\n";
                    if(line.substr(0,4)=="PPid")
                        cout<<line<<"\n";
                    if(line.substr(0,3)=="Uid")
                    {
                        buffer<<line;
                        buffer>>a>>b>>c>>d>>e;
                        cout<<"Real UID:"<<b<<"\n";
                        cout<<"Effective UID:"<<c<<"\n";
                        cout<<"Saved set:"<<d<<"\n";
                        cout<<"Filesystem UID:"<<e<<"\n";
                        buffer.clear();
                    }
                    if(line.substr(0,3)=="Gid")
                    {
                        buffer<<line;
                        buffer>>a>>b>>c>>d>>e;
                        cout<<"Real GID:"<<b<<"\n";
                        cout<<"Effective GID:"<<c<<"\n";
                        cout<<"Saved set:"<<d<<"\n";
                        cout<<"Filesystem GID:"<<e<<"\n";
                        buffer.clear();
                    }
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="State")
        {
            stringstream buffer;
            string line;
            file_name= "//proc//" +pid + "//stat";
            
            ifstream file(file_name.c_str());
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                if(!file.eof())
                {
                    string a,b,c;
                    getline(file,line);
                    buffer<<line;
                    buffer>>a>>b>>c;
                    cout<<"States:"<<c<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="Thread_Information")
        {
            string line;
            file_name= "//proc//" +pid + "//status";
            
            ifstream file(file_name.c_str());
            
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                while(!file.eof())
                {
                    getline(file,line);
                    
                    if(line.substr(0,7)=="Threads")
                        cout<<line<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
         if(request=="Priority")
        {
            string a;
            stringstream buffer;
            string line;
            file_name= "//proc//" +pid + "//stat";
            
            ifstream file(file_name.c_str());
            
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                getline(file,line);
                buffer<<line;
                for(int x=0;x<19 ;x++)
                {
                    buffer>>a;
                    if(x==17)
                        cout<<"Priority Number "<<a<<"\n";
                    if(x==18)
                        cout<<"Nice Value "<<a<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="Time_Information")
        {
            string a;
            stringstream buffer;
            string line;
            file_name= "//proc//" +pid + "//stat";
            
            ifstream file(file_name.c_str());
            
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                getline(file,line);
                buffer<<line;
                for(int x=0;x<18 ;x++)
                {
                    buffer>>a;
                    if(x==13)
                        cout<<"utime: "<<a<<"\n";
                    if(x==14)
                        cout<<"stime: "<<a<<"\n";
                    if(x==10)
                        cout<<"cutime "<<a<<"\n";
                    if(x==16)
                        cout<<"cstime "<<a<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="Address_Space")
        {
            string a;
            stringstream buffer;
            string line;
            file_name= "//proc//" +pid + "//stat";
            
            ifstream file(file_name.c_str());
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                getline(file,line);
                buffer<<line;
                for(int x=0;x<30 ;x++)
                {
                    buffer>>a;
                    if(x==24)
                        cout<<"Startcode: "<<a<<"\n";
                    if(x==21)
                        cout<<"Endcode: "<<a<<"\n";
                    if(x==28)
                        cout<<"ESP: "<<a<<"\n";
                    if(x==29)
                        cout<<"EIP: "<<a<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="Resources")
        {
            string line;
            file_name= "//proc//" +pid + "//status";
            
            ifstream file(file_name.c_str());
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                while(!file.eof())
                {
                    getline(file,line);
                    
                    if(line.substr(0,9)=="voluntary")
                        cout<<line<<"\n";
                    if(line.substr(0,12)=="nonvoluntary")
                        cout<<line<<"\n";
                    if(line.substr(0,6)=="FDSize")
                        cout<<line<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="Processors")
        {
            string a;
            stringstream buffer;
            string line;
            file_name= "//proc//" +pid + "//stat";
            
            ifstream file(file_name.c_str());
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                getline(file,line);
                buffer<<line;
                for(int x=0;x<39 ;x++)
                {
                    buffer>>a;
                    if(x==38)
                        cout<<"Last Core the process was last executed: "<<a<<"\n";
                }
                file.close();
            }
            file_name="";
            
            file_name= "//proc//" +pid + "//status";
            file.open(file_name.c_str());
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                
                while(!file.eof())
                {
                    getline(file,line);
                    
                    if(line.substr(0,17)=="Cpus_allowed_list")
                        cout<<line<<"\n";
                }
                file.close();
            }
            file_name="";
        }
        
        if(request=="Memory_map")
        {
            string line;
            stringstream buffer;
            file_name= "//proc//" +pid + "//maps";
            
            ifstream file(file_name.c_str());
            if(!file.is_open())
            {
                cout<<"not a correct pid \n";
                cout<<"try to insert a new pid with the command pid\n";
            }
            else
            {
                string add,perm,offset,dev,inode,path;
                
                while(!file.eof())
                {
                    getline(file,line);
                    buffer<<line;
                    buffer>>add>>perm>>offset>>dev>>inode>>path;
                    if(!buffer.fail())
                    {
                        if(path=="[heap]"||path=="[stack]"||path=="[vdso]"||path=="[vsyscall]")
                        cout<<setw(10)<<"Address range: "<<add
                            <<setw(20)<<" Permissions: "<<perm
                            <<setw(10)<<" offset: "<<offset
                            <<setw(10)<<" dev: "<<dev
                            <<setw(10)<<" inode: "<<inode
                            <<setw(10)<<" path name: "<<path<<setw(10)<<"\n";
                    }
                    buffer.clear();
                }
                file.close();
            }
            file_name="";
        }
    }
    while(request!="exit");
}