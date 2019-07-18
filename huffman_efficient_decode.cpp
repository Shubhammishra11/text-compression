#include<bits/stdc++.h>
using namespace std;
#define int long long int 
#define MAX_NUM 256

struct node{
	char alpha;
	struct node *left, *right;
	node(char alpha){
		this->alpha=alpha;
		this->right=this->left=NULL;
	}
};

struct node *root=new node('#');





//-------------building huffman tree used for encoding-------------------//

void build_tree(char alpha,string bit_rep){
	struct node *temp=root;
	for(unsigned i=0;i<bit_rep.size();i++){
		if(bit_rep[i]=='0'){
			if(temp->left==NULL)
			temp->left=new node('#');
			temp=temp->left;
		}
		else {
			if(temp->right==NULL)
			temp->right=new node('#');
			temp=temp->right;
		}

	}
	temp->alpha=alpha;
	
}

//---------------decoding .bin file-------------------------//

void read_decoded_file(string input,string output){
	FILE *fr=fopen(input.c_str(),"rb");
	FILE *fout=fopen(output.c_str(),"w");
	
	char c;
	struct node *temp=root;
	while((c=fgetc(fr))!=EOF){
		
		for(int i=6;i>=0;i--){
			
			int x=((c>>i)&1);
			
			if(x){
				temp=temp->right;
				if(temp->alpha!='#'){
					fprintf(fout,"%c",temp->alpha);
					temp=root;
				}
			}
			else{
				temp=temp->left;
				if(temp->alpha!='#'){
					fprintf(fout,"%c",temp->alpha);
					temp=root;
				}
			}
			
			
		}
	}
}

void decode(string input,string output,string frequency_file){
	FILE *fr=fopen(frequency_file.c_str(),"r");
		
	char c;
	while((c=fgetc(fr))!=EOF){
		
		char in=c;string bit_rep;	
		c=fgetc(fr);	
		while((c=fgetc(fr))!='\n')bit_rep+=c;
		
		build_tree(in,bit_rep);

	}
	read_decoded_file(input,output);
}

//----------------------------------------------------------------//

int32_t main(int32_t argc,char** argv){
	clock_t time=clock();
	if(argc!=4)
		fprintf(stderr,"./huffman_decode.out <frequency_file>"
					"<decoded_bin_file> <output file> ");
	string frequency_file=argv[1];
	string decoded_file=argv[2];
	string output_file=argv[3];
	
	decode(decoded_file,output_file,frequency_file);
	cout<<(clock()-time)*1.0/CLOCKS_PER_SEC;

	return 0;
}
