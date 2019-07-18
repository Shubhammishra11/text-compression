#include<bits/stdc++.h>
using namespace std;
#define int long long int
#define MAX_NUM 256

int frequency[MAX_NUM];

unordered_map<int,string> table;
int bit_counter;
unsigned char cur_byte;


struct node{
	char alpha;int weight;
	struct node *left, *right;
	node(char alpha,int weight){
		this->alpha=alpha;
		this->weight=weight;
		this->right=this->left=NULL;
	}
};

//--------member function for precalculating the frequency distribution of text in file--------//
void determine_frequency(FILE *fin){
	char c;
	while((c=fgetc(fin))!=EOF){
		++frequency[(int)c];
		
	}
	
}

// comparator function for priority queue

struct compare { 
    bool operator()(struct node* l, struct node* r){
      return (l->weight > r->weight);
    }
};

//---------- getting the codes corresponding to each alphabet used-------------// 

void getcodes(struct node* root,string s){
	if(!root)
		return;
	if(root->alpha!='#')
		table[(int)root->alpha]=s;
	getcodes(root->left,s+"0");
	getcodes(root->right,s+"1"); 
}

// --------------building the huffman tree----------------------//

void build_tree(){
	priority_queue<struct node*,vector<struct node*>,compare> priority;
	for(int i=0;i<MAX_NUM;i++){
		if(frequency[i]>0){
			
			priority.push(new node((char)i,frequency[i]));
		}
	}
	while(priority.size()!=1){
		struct node *node1,*node2;
		node1=priority.top();
		priority.pop();
		node2=priority.top();
		priority.pop();
		struct node *new_node=new node('#',node1->weight+node2->weight);
		new_node->left=node1;new_node->right=node2;
		priority.push(new_node);
	}
	getcodes(priority.top(),"");
}

//-------------writing alphabet codes distribution file----------------------//
void write_frequency_file(FILE *&fr){
	
	for(int i=0;i<MAX_NUM;i++){
		if(frequency[i]>0){
			fprintf(fr,"%c %s\n",(char)i,table[i].c_str());
	}}

}

// ------------------------writing encoded .bin file------------------------//
void write_bit(unsigned char bit,FILE *&f)
{	
    if(bit_counter == 7)
    {	
        fwrite(&cur_byte,1,1,f);
        bit_counter = 0;
        cur_byte = 0;
       
    }
  
    bit_counter++;
    cur_byte <<= 1;
    cur_byte |= bit;
}


void write_encoded_file(FILE *&fout,FILE *&fin){
	char c;
	cur_byte=0;
	bit_counter=0;
	while((c=fgetc(fin))!=EOF){
		string s=table[(int)c];
		for(unsigned int i=0;i<s.size();i++)
			write_bit((int)s[i]-48,fout);

	}
	

	if(bit_counter > 0)
    {
         
         cur_byte <<= 7 - bit_counter;
         fwrite(&cur_byte, 1, 1, fout);
    }
}

void encode(string original,string decoded){
	FILE *fr=fopen("frequency.txt","w");
	FILE *fin=fopen(original.c_str(),"r");
	FILE *fout=fopen(decoded.c_str(),"wb");
	
	determine_frequency(fin);
	build_tree();
	fseek(fin,0,SEEK_SET);
	
	write_frequency_file(fr);
	write_encoded_file(fout,fin);
	
	
	
	fclose(fout);
	fclose(fin);
	fclose(fr);
}

//----------------------------------------------------------//
int32_t main(int32_t argc,char **argv){
	clock_t time=clock();
	if(argc!=3)
		fprintf(stderr,"./huffman_encode.out"
						" <original_file_name> <decoded_bin_file_name>\n");
	string original=argv[1];
	string decoded=argv[2];
	encode(original,decoded);
	cout<<(clock()-time)*1.0/CLOCKS_PER_SEC;
	return 0;
}