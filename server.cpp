// author Manaranjan Saren
//Redis Implementation
// Server side Redis implementation in c++
//Handle multiple socket connections with select and fd_set on Linux 

#include<bits/stdc++.h>
using namespace std;
#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds;
#define pii pair<int,string>  
#define ordered_set tree<pii, null_type,less<pii>, rb_tree_tag,tree_order_statistics_node_update> 
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <time.h>
#include <chrono>
#define TRUE 1 
#define FALSE 0 
#define MAX 1025



unordered_map<string,string> mp;
unordered_map<string,ordered_set> mp1;
unordered_map<string,unordered_map<string,int> > mp2;
unordered_map<string,string> time_map;
unordered_map<string,int> starting_time;


auto start_time = std::chrono::high_resolution_clock::now();

string GET(string key)
{
    if(mp.find(key)==mp.end())
      return "nil";
    //cout<<mp[key]<<"get\n";
    //time_map["tt"]="5";
    if(time_map.find(key)!=time_map.end())
    {
    	 string x=time_map[key];
    	 int tm=stoi(x);
    	 auto current_time = std::chrono::high_resolution_clock::now();

    	 int time_lapse=std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
         //std::cout << "Program has been running for " <<time_lapse<< " seconds" << std::endl;
         time_lapse-=starting_time[key];
    	 if(time_lapse > tm)
    	 {  
              time_map.erase(key);
              mp.erase(key);
              return "nil";
    	 }
    }
    return mp[key];
}
void SET(string key,string val)
{
	 mp[key]=val;
     
	 /*cout<<"values inside map"<<"\n";
	 for(auto it=mp.begin();it!=mp.end();it++)
	 {
	 	 cout<<it->first<<" "<<it->second<<endl;
	 }*/

	
}
string EXPIRE(string key,string tm)
{
	 string ans;
	 if(mp.find(key)==mp.end())
	 {
	 	 ans.push_back('0');

	 }
	 else
	 {
          time_map[key]=tm;
          ans.push_back('1');
          auto current_time = std::chrono::high_resolution_clock::now();
    	  int time_lapse=std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
          starting_time[key]=time_lapse;
	 }
	 return ans;
}

void showsetcontainings()
{
	cout<<"all set contains\n";
	for(auto it=mp1.begin();it!=mp1.end();it++)
	{
		 cout<<"setname"<<" "<<(it)->first<<"\n";
		 cout<<"elements of set\n";
         for(auto it2=(it)->second.begin();it2!=(it)->second.end();it2++)
         {
         	 cout<<(it2)->first<<" "<<(it2)->second<<"\n";
         }
	}
}
void ZADD(string setname,string a,string key)
{
	int score=stoi(a);
	if(mp2.find(setname)==mp2.end())
	{
		
		  mp2[setname][key]=score;
		  mp1[setname].insert(make_pair(score,key));
		 
	}
	else
	{
         if(mp2[setname].find(key)==mp2[setname].end())
		 {
		  mp2[setname][key]=score;
		  mp1[setname].insert(make_pair(score,key));
		 }
		 else
		 {
		 	 int prev_score=mp2[setname][key];
		 	 pair<int,string> p=make_pair(prev_score,key);
		 	 mp1[setname].erase(p);
		 	 mp1[setname].insert(make_pair(score,key));
		 }
	}
	
}
string ZRANK(string setname,string key)
{
	string ans;
   if(mp2.find(setname)==mp2.end())
   {
         ans="nil";
   }
   else
   	if(mp2[setname].find(key)==mp2[setname].end())
   	{
   		 ans="nil";
   	}
   	else
   	{
   		 int val=mp2[setname][key];
   		 int rank=mp1[setname].order_of_key(make_pair(val,key));
   		 ans=to_string(rank);
   	}
   	return ans;
}
vector<string> ZRANGE(string setname,string a,string b,string c)
{
	 int start=stoi(a);
	 int stop=stoi(b);
	 int flag=stoi(c);
	 vector<string> ans;
     int sz=mp1[setname].size();
     if(start<0)
        start=sz+start;
      if(stop < 0)
        stop=sz+stop;

    if(mp2.find(setname)==mp2.end())
    	return ans;
     
      pii p1=*(mp1[setname].find_by_order(start));
      pii p2=*(mp1[setname].find_by_order(stop));
      auto it1=mp1[setname].find(p1);
      auto it2=mp1[setname].find(p2);
      it2++;
	  if(flag==1) // withscores
	  {
          
         for(;it1!=it2 && it1!=mp1[setname].end();it1++)
         {
         	string a=(*it1).second;
         	a.push_back('\n');
         	 ans.push_back(a);
         	 int x=(*it1).first;
         	 string s=to_string(x);
         	 s.push_back('\n');
         	 ans.push_back(s);
         }
        
         

	  }
	  else
	  {
	  	 for(;it1!=it2;it1++)
         {
         	 
         	 int x=(*it1).first;
         	 string s=to_string(x);
         	 ans.push_back(s);
         }
	  }

	  return ans;
}
string change_to_small_case(string command)
{
	  string ans;
	  for(char c:command)
	  {
	  	 c=tolower(c);
	  	 ans+=c;
	  }
	  return ans;
} 
vector<string> execute_command(vector<string> CM)
{
     vector<string> ans;
     
     string command=change_to_small_case(CM[0]);
     //cout<<command<<"command\n";
     if(command=="get")
     {
     	  //cout<<CM[1]<<"check get commad\n";
     	  string val=GET(CM[1]);
     	  //cout<<"val"<<val<<"\n";
     	  ans.push_back(val);
     }
     else
      if(command=="set")
      {
      	  //cout<<CM[1]<<" "<<CM[2]<<"\n";
      	  SET(CM[1],CM[2]);

      }
      else
      	if(command=="expire")
      	{
      		  string val=EXPIRE(CM[1],CM[2]);
      		  ans.push_back(val);
      	}
      else
      	if(command=="zadd")
      	{
      		  ZADD(CM[1],CM[2],CM[3]);
      	}
      	else
      	  if(command=="zrank")
      	  {
      	  	  string rank=ZRANK(CM[1],CM[2]);
      	  	  ans.push_back(rank);
      	  }
      	  else
      	  	if(command=="zrange")
      	  	{
                ans=ZRANGE(CM[1],CM[2],CM[3],CM[4]);
      	  	}

      	  	return ans;
     

}

void server_closing()
{
	
   std::ofstream ofs;
   ofs.open("database.txt", std::ofstream::out | std::ofstream::trunc);
   ofs.close();
   
     ofstream myfile;
	 myfile.open("database.txt");
	 

   for(auto it=mp.begin();it!=mp.end();it++)
   {
     myfile << it->first <<"\n";
	 myfile << it->second <<"\n";
   }
   myfile.close();

   ofs.open("setbase.txt", std::ofstream::out | std::ofstream::trunc);
   ofs.close();
   
   myfile.open("setbase.txt");

   for(auto it=mp1.begin();it!=mp1.end();it++)
   {
   	   string setname=it->first;
   	   myfile << setname <<"\n";
   	   int sz=it->second.size();
   	   string ssz=to_string(sz);
   	   myfile << ssz <<"\n";

   	   for(auto it2=it->second.begin();it2!=it->second.end();it2++)
   	   {
   	   	  myfile << it2->first <<"\n";
   	   	  myfile << it2->second <<"\n";

   	   }
   }





  
}
int change_to_int(string s)
{
	 int ans=0;
	 for(char c:s)
	 {
	 	 if(c>='0' && c<='9')
	 	 {
	 	 	 ans=10*ans+(c-'0');
	 	 }
	 }
	 return ans;
}
void server_opening()
{
  //cout<<"redis started\n";
  string line;
  ifstream myfile ("database.txt");
  int cnt=0;
  string a,b;
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
        if(cnt==0)
        {
        	 a=line;
        }
        else
         b=line;
         
        cnt++;
        if(cnt==2)
        {
        	cnt=0;
        	mp[a]=b;
        } 	
      

    }
    myfile.close();
  }

  // updating the sorted set

  ifstream my ("setbase.txt");
  cnt=0;
  
  int sz=0;
  int bb=0;
  int x=0;
  if (my.is_open())
  {
    while ( getline (my,line) )
    {
        
       if(cnt==0)
       {
       	 a=line;
       	 //cout<<line<<"\n";
       	 cnt++;

       }
       else
       	if(cnt==1)
       	{
       		 string ln=line;
       		 
       		 sz=change_to_int(ln);
       		 //cout<<cnt<<" "<<sz<<"\n";
       		 cnt++;


       	}
       	else
       	  if(cnt==2*sz+1)
       	  {
       	  	  mp1[a].insert(make_pair(bb,line));
       	  	  //cout<<line<<"\n";
       	  	  mp2[a][line]=bb;
       	  	  cnt=0;
       	  	  x=0;
       	  }
       	  else
       	  {
       	  	  if(x==0)
       	  	  {
       	  	  	 string kk=line;
       	  	  	 bb=change_to_int(kk);
       	  	  	 //cout<<cnt<<" "<<bb<<"\n";
       	  	  	 //cout<<line<<"\n";
       	  	  	 x++;
       	  	  	 cnt++;
       	  	  }
       	  	  else
       	  	  {
       	  	  	 mp1[a].insert(make_pair(bb,line));
       	  	  	 mp2[a][line]=bb;
       	  	  	 //cout<<line<<"\n";
       	  	  	 x=0;
       	  	  	 cnt++;
       	  	  }
       	  }

       	  

    }
    my.close();
  }

//showsetcontainings();



}

vector<string> parse_commands(string s)
{
	 vector<string> v;
	 string f;
	 //cout<<s.length()<<"\n";
	 for(int i=0;i<s.length();i++)
	 {
	 	 if(s[i]=='\n')
	 	 	break;
	 	 if(s[i]==' ')
	 	 {
	 	 	 v.push_back(f);
	 	 	 f.clear();

	 	 }
	 	 else
	 	 {
	 	 	 f.push_back(s[i]);
	 	 }
	 }
	 if(f.length()>=1)
	 {
	 	v.push_back(f);
	 }

	 //for(string k:v)
	 	//cout<<k<<"end\n";
	 
	 return v;
}
int main(int argc , char *argv[]) 
{ 

    
	server_opening();
    int PORT;

    string port=argv[1];
    PORT=stoi(port);
	
	
   
	int ret=-1;
	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[30] , 
		max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char buffer[1025]; //data buffer of 1K 
    char leave[1025]="exit\n";
    char ans[1025];
    char arr[1025];
	//set of socket descriptors 
	fd_set readfds; 
		
	//a message 
	char message[1025] = "Welcome to Redis"; 
	//printf("%s\n", message);
	
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
		
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
	//fd_set magic;
	//struct timeval tv;
	int cnt=0;
	while(TRUE) 
	{ 
     //memset(array, 0, 1025);

	       //FD_ZERO(&magic);
           //FD_SET(0, &magic);
           //tv.tv_sec = 5;
           //tv.tv_usec = 0;
           //int retval = select(1, &magic, NULL, NULL,&tv);
           // if (retval == -1)
               //perror("select()");
           //else if (retval)
           //{
           	    
          //}
                
     

		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 


printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port)); 
		
			//send new connection greeting message 
			if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
			{ 
				perror("send"); 
			}
				
			//puts("Welcome message sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i); 
					cnt++;
						
					break; 
				} 
			} 
		} 
	   
		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &readfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming message 
				memset(buffer, 0, 1025);
				read( sd , buffer, 1025);
                ret=strcmp(buffer,leave);
				//printf("%s\n", buffer);
				//cout<<ret<<"\n";
				if ( ret<=0 ) 
				{ 
					//cout<<"byye";
					cnt--;
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				//Echo back the message that came in 
				else
				{ 
					//set the string terminating NULL byte on the end 
					//of the data read 
					//buffer[valread] = '\0'; 
                    
                    string cmds=buffer;
                    vector<string> v=parse_commands(cmds);
                    

                   memset(ans, 0, 1025);
                   
                   vector<string> vv=execute_command(v);

                   
                   	 ans[0]='1';
                   	 
                   
                   
                    int k=0;
                    for(string h:vv)
                    {
                    	 //cout<<h<<"\n";
                    	 for(char c:h)
                    	 {
                    	 	 ans[k]=c;
                    	 	 k++;
                    	 }
                    }
                    if(k==0)
                    	k++;
                    ans[k]='\n';
                    //printf("%s",buffer);*/

					send(sd , ans, strlen(ans) , 0 ); 
				} 
			} 
		}

		if(cnt==0)
		{
			cout<<"There is no clients present do you want to close\n";
			cout<<"Type exit to close or for else type anything \n";
		      fgets(arr, MAX, stdin);
                ret=strcmp(arr,leave);
                if(ret==0)
                {
     		      cout<<"closing.....";
	               server_closing();
	               break;

     	        }
     	}        

	} 
   
		
	return 0; 
} 
