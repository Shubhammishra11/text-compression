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
    struct node* top;
    
 
    // Create a vector & inserts all characters of data[]
    vector<struct node*> minHeap;
 
    for (int i = 0; i < MAX_NUM; ++i)
        if(frequency[i]>0)
           minHeap.push_back(new node((char)i, frequency[i]));
 
    // Iterate while size of heap doesn't become 1
    vector<struct node*>:: iterator it;
    struct node* temp1;
    struct node* temp2;
    vector<struct node*>:: iterator it1;
    vector<struct node*>:: iterator it2;
    int mx;
    while (minHeap.size() != 1) { 
        // Extract the two minimum
        // freq items from vector
        mx=1e5;
        int mxfreq=0;
        for(it=minHeap.begin();it!=minHeap.end();++it){
            if((*it)->weight<mx){
                mx=(*it)->weight;
                it1=it;
            }
        }
        temp1=*it1;   
        mxfreq+=mx;
        minHeap.erase(it1);
        mx=1e5;
        for(it=minHeap.begin();it!=minHeap.end();++it){
            if((*it)->weight<mx){
                mx=(*it)->weight;
                            it2=it;
            }
        }
        temp2=*it2;
        mxfreq+=mx;
        minHeap.erase(it2);
        top = new node('#', mxfreq);
        top->left = (temp1);
        top->right = (temp2);
        minHeap.push_back(top);
    }
   
    getcodes(*minHeap.begin(), "");
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