#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <limits>
#include <openssl/sha.h>
bool DEBUG=true;
int NONCE_LENGTH=5;
using namespace std;
void printwelcome()
{
	printf("Welcome to the transaction-chain application!\n");
	printf("1) Add a transaction to the chain!\n");
	printf("2) Find a transaction with name!\n");
	printf("3) Verify and pritn the chain!\n");
	printf("Which operation do you want to make? (1,2,3)\n");
}
void GetNonce(char* nonce)
{	
	cout << "\nIn side GetNonce\n";
	//srand(time(NULL));
	srand(time(NULL)*436);
    rand();// first rand() to boost the randomness; other wise the generated seed doesn't change too much
    rand();// first rand() to boost the randomness; other wise the generated seed doesn't change too much
	for(int i=0;i<NONCE_LENGTH;++i)
	{
		//cout << "i:";
		//cout << char(rand()%26+97) << "\n";
		nonce[i]=char(rand()%26+97);
		//cout << "nonce:" << i << nonce[i] << "\n"; 
	
	}
	return;
}
string GetHashNonce(int money,string send, string receive, char* mdString, char* nonce)
{
	bool ONCE_FLAG=true;
	//char mdString[SHA256_DIGEST_LENGTH*2+1];
	unsigned char digest[SHA256_DIGEST_LENGTH];
	ostringstream stringmoney;
	stringmoney << money;
	string hashstring=stringmoney.str();
    hashstring +=send+receive;
    string hash_temp;
    string nonce_appended;
    hash_temp=hashstring;
//    srand(time(NULL));
//    rand();// first rand() to boost the randomness; other wise the generated seed doesn't change too much
    //Check whether once satisfy the requirement
    while(ONCE_FLAG){
//    for(int i=0;i<NONCE_LENGTH;++i)
//	{
		//cout << "i:";
		//cout << char(rand()%26+97) << "\n";
//		nonce[i]=char(rand()%26+97);
		//cout << "nonce:" << i << nonce[i] << "\n"; 
	
//	}
    GetNonce(nonce);
//    if(DEBUG){
//   cout << "\nnonce:" << nonce << "\n";}
//   cout << "\nlength nonce:" << strlen(nonce);
    //hash_temp=hashstring;
    hash_temp.append(nonce,NONCE_LENGTH);
    nonce_appended.append(nonce,NONCE_LENGTH);
    if(DEBUG){cout << "\nFinal hash_temp:"<< hash_temp << "   lenght"<< hash_temp.length() << "\n";}
    if(DEBUG){cout << "\nFinal nonce_appended:"<< nonce_appended << "   lenght"<< nonce_appended.length() << "\n";}
    SHA256((unsigned char*)&hash_temp, hash_temp.length(), (unsigned char*)&digest);
//    cout << "\n digest length" << SHA256_DIGEST_LENGTH; 
//    cout << "\n digest length\n" << strlen(digest);
    //cout << "\n digest \n" << digest;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    	/* Convert the unsigned int in hexadecimal format to char and store it in variable 'mdString' */
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    }
//    if(DEBUG){cout << "\n mdString:" << mdString; }
    
    if(mdString[strlen(mdString)-1]=='0' || mdString[strlen(mdString)-1]=='1' || mdString[strlen(mdString)-1]=='2' || mdString[strlen(mdString)-1]=='3'||mdString[strlen(mdString)-1]=='4')
    {ONCE_FLAG=false;}
    }
    //char char_array[hash_temp.length()+1];
    //strcpy(char_array,hash_temp.c_str());
    //nonce=char_array; 
   // if(DEBUG)
   // {
   // printf("DEBUG SHA256 digest hexadecimal format: %s\n\n", digest);
	//}

   // for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    	/* Convert the unsigned int in hexadecimal format to char and store it in variable 'mdString' */
    //    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    //}
    
    //if(DEBUG)
    //{    printf("DEBUG SHA256 digest: %s\n", mdString);}
	return nonce_appended;

}
class Transaction
{
	private:
	Transaction* next;	//points previous transaction
	int amount;			// amount of money that has been transferred
	string sender;	 
	string receiver;
	string nonce;		//*random string that will be used during hashing
	string hashing;		// hash of content of previous transaction
	
	public:
	string nexthashing;
	Transaction() 
	{	next=NULL; amount=0; sender="sender"; receiver="receiver"; nonce="nonce"; hashing="hashing"; 		    
		nexthashing="";}// just for debuging
//	Transaction() {next=NULL; amount=0; sender="sender"; receiver="receiver"; nonce="nonce"; hashing="hashing"; }// just for debuging
	//TODO: Add random nonce & hashing
	Transaction(Transaction* header,int money,string send,string receive) :next(header), amount(money), sender(send), receiver(receive) {
		//unsigned char digest[SHA256_DIGEST_LENGTH];
		//ostringstream stringmoney;
		//stringmoney << money;
		//string hashstring=stringmoney.str();
		char mdString[SHA256_DIGEST_LENGTH*2+1];
		char nonce_temp[NONCE_LENGTH];
		if (header==NULL)
		{
			if(DEBUG){
			if(hashing.empty())
				cout << "Hashing is empty\n";
			}
			//cout << "nonce_temp" << nonce_temp << "\n";
			//GetNonce(nonce_temp);
			//cout << nonce_temp << "\n";
			nonce=GetHashNonce(money,send,receive,mdString,nonce_temp);
			hashing="";
			//nonce=nonce_temp;
			nexthashing=mdString;
			//hashstring = hashstring+send+receive;
			//SHA256((unsigned char*)&hashstring, hashstring.length(), (unsigned char*)&digest);
			if(DEBUG)
			{	printf("SHA256 digest int format: %s\n\n", mdString);
				printf("SHA256 nonce_temp: %s\n\n", nonce_temp);
				cout << "SHA256 nonce:"<<nonce <<"\n";
			}
		}else
		{
			hashing=header->nexthashing;
			nonce=GetHashNonce(money,send,receive, mdString,nonce_temp);
			nexthashing=mdString;			
		}
	
	
	
	}
	//TODO: Add destructor
	~Transaction(){}
	Transaction* GetNext()
	{
		return next;
	}
	string GetSender()
	{
		return sender;
	}
	void printTransaction()
	{
		cout<<"==========Amount=============="<<amount<<"\n";
		cout<<"==========Sender=============="<<sender<<"\n";
		cout<<"==========Receiver============"<<receiver<<"\n";
		cout<<"==========Nonce==============="<<nonce<<"\n";
		cout<<"==========Hashing============="<<hashing<<"\n";
		cout<<"==========Nexthashing============="<<nexthashing<<"\n";
		return ;
	}


};

class BlockChain
{
	public:
	int length;	
	Transaction* head;
	BlockChain(){}
	~BlockChain(){}
	void AddTransaction()
	{
		Transaction* tran = new Transaction(this->head,1000,"miumiu","saolei");
		this->head = tran;
		this->length++;
	}
	void PrintBlockChain()
	{
		Transaction* printhead = this->head;
		int i = 1;
		while(printhead)
		{
			cout << i << ":\n";
			i++;
			printhead->printTransaction();
			printhead = printhead->GetNext();
		}
	
	
	}
    void FindTransaction(string sender)
	{
		Transaction* printhead = this->head;
		int i = 1;
		string temp_sender;
		while(printhead)
		{
			cout << i << ":\n";
			i++;
			temp_sender=printhead->GetSender();
			if(temp_sender.compare(sender)==0){
			printhead->printTransaction();}
			printhead = printhead->GetNext();
		}
	
	
	}
	
};

int main()
{
	int a;
	int money=100;
	string hashstring;
	string send="miumiu", receive="saolei";
	unsigned char digest[SHA256_DIGEST_LENGTH];
	ostringstream stringmoney;
	
	BlockChain* blockchain = new BlockChain();
	// Three different initilization method
	//Transaction tran= Transaction();
	//Transaction* head= new Transaction();
	//Transaction tran1= Transaction(head,100,"Leilai","saolei");
	//Transaction* tran2= new Transaction();
	while(1){
		printwelcome();
		try {
			cin >> a;
			if(cin.fail()){ throw range_error("========Input is not an interger==========\n");
			} else{	
					if(a>3 || a<1){ throw range_error("=========Input must among 1,2,3==============\n");
					}else{
						printf("========Your choice is %d=============\n",a);
						switch(a){
						case 1: 
						printf("========Add one more transaction==========\n");
						//TODO: Add input try & catch
						blockchain->AddTransaction();
						if(DEBUG)
						{
						blockchain->PrintBlockChain();// Debuging only
						}
//						stringmoney << money;
//						hashstring=stringmoney.str();
//						hashstring = hashstring+send+receive;
//						SHA256((unsigned char*)&hashstring, hashstring.length(), (unsigned char*)&digest);
//						cout << "SHA256 digest hexadecimal format:"<< digest << "\n\n";
						//tran.printTransaction();
						break;
						case 2: printf("========Find transactions by name========\n");
						//tran1.printTransaction();
						//TODO: Add input try & catch
						if(blockchain->head==NULL){cout << "==============Empty blockchain==============\n";}
						else{
						blockchain->FindTransaction("miumiu");
						}
						break;
						case 3: printf("======Verify and Print the Block chain=====\n");
						if(blockchain->head==NULL){cout << "==============Empty blockchain===============\n";}
						else{
						blockchain->PrintBlockChain();// Debuging only
						}
						break;
						
						default:throw range_error("======Switch Case not among 1/2/3=======\n");

						}
					}
				}
		}
		catch (const runtime_error& e) {
			cout << e.what() << "\n";
			cin.clear();//clear error flags
			cin.ignore(numeric_limits<streamsize>::max(),'\n');// clear out the input buffer
		}
	}
}
