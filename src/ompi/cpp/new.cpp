#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <bitset>
#include <ctime>
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>
#include <map>
using namespace std;


class generator
{	
	string alf; 
	map <char, int> mp; 
	string value; 

public:
	generator(string alf, int len);
	void head();
	string get_string();
	
};

generator::generator(string _alf, int len)
{
	alf = _alf;
	srand(time(NULL));

	string alfp = "";
	int count = 1;
	for (char letter : alf)
	{
		for (int count_letter = 0; count_letter < count; count_letter++) {
			alfp.push_back(letter);
		}
		count++;
	}

	
	for (char i : alf) { mp[i] = 0; }
	int elem;
	for (int i = 0; i < len; i++){
		elem = alfp[rand() % alfp.length()];
		value.push_back(elem);
		mp[elem]++;
	}
	
}

void generator::head()
{	
	cout  << "alphabet: " ;
	if (value.length() < 60) {
		std::cout << value;
	}
	else {
		std::cout << value.substr(0, 60);
	}
	std::cout <<"..." << endl;
	cout  << "P: \n";
	for (auto i : mp)
		cout << "\t"  << i.first << ": " << i.second<< endl;

}

string generator::get_string()
{
	return value;
}


struct Node
{
    char ch;
    int freq;
    Node* left, * right;
};

Node* getNode(char ch, int freq, Node* left, Node* right);

class Coder
{
	void opener(bool f = 0);
	void opener_RLE(bool f = 0);
    Node* root = NULL;

public:
	
	std::pair<string, Node*> Huffman(bool action, string text);
	std::string RLE(bool action, std::string text);
};



struct comp
{

    bool operator()(const Node* l, const Node* r) const
    {
        return l->freq > r->freq;
    }
};

bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

void encode(Node* root, string str, unordered_map<char, string>& huffmanCode)
{
    if (root == nullptr) {
        return;
    }

    
    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != "") ? str : "1";
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int& index, string str, string &res)
{  
    if (root == nullptr) {
        return;
    }

    if (isLeaf(root))
    {
        res.push_back(root->ch);
        return ;
    }

    index++;

    if (str[index] == '0') {
        decode(root->left, index, str, res);
    }
    else {
        decode(root->right, index, str, res);
    }
   
}

Node* buildHuffmanTree(string text)
{
    if (text == "") { return NULL; }


    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    prioritY\_queue<Node*, vector<Node*>, comp> pq;

    for (auto pair : freq) { pq.push(getNode(pair.first, pair.second, nullptr, nullptr)); }

    while (pq.size() != 1)
    {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top();    pq.pop();
        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }


    Node* root = pq.top();
    return root;
}

vector<char> encode_RLE(string &txt) {

    
    std::vector<char> res;

    int index = 0;
    
    while (index < txt.length()) {

        char symbol = txt[index];
        char len_series = 0;

        while (symbol == txt[index + len_series]) {   
            len_series++;
            if (len_series == 127) { break; } 
        }
        if (len_series == 1) {
            vector<char> tmp = { symbol };
            while ( (txt.length() > (index + len_series + 1)  && txt[index + len_series] != txt[index + len_series + 1]  )) {
                tmp.push_back(txt[index + len_series]);
                len_series++;
            }

            res.push_back(len_series + 128);
            for (auto c : tmp) { res.push_back(c);  }
            index = index + len_series;
            continue;
        }

        res.push_back(len_series);
        res.push_back(symbol);  
        index = index + len_series;
    }
    return res;
}

string decode_RLE(string& txt, string &separator) {
    string res;
    
    vector<char> out(txt.begin(), txt.end());

    int n;
    bool flag = true;
    for (int i = 0; i < out.size(); i++) {
        n = bitset<8>(out[i]).to_ulong();

        if (n < 128) {
            i++;

            for (int j = 0; j < n; j++) {
                res += char(bitset<8>(out[i]).to_ulong());
            }
        }
        else {
            n = n - 128;
            i++;
            for (int j = i; j < i + n; j++) {
                res += char(bitset<8>(out[j]).to_ulong());
            }
            i = i + n - 1;

        }

    }
    return res;
}

pair<string, Node*> Coder::Huffman(bool action, string text )
{
    
	string res;
	if (action == 0) {
        
        float cod_price = 0;
		
	    root = buildHuffmanTree(text);
       
        unordered_map<char, string> huffmanCode;
        encode(root, "", huffmanCode);

        //cout <<"\t" << "Cods: ";
        for (auto pair : huffmanCode) {
            //cout << pair.first << "-" << pair.second << " ";
            
            float count_char = count(text.begin(), text.end(), pair.first);
            float p_char = count_char / text.length();
            cod_price += p_char * pair.second.length();
        }
        cout << endl;



        
        string str;
        for (char ch : text) {   str += huffmanCode[ch];   }

        cout << "\t" << "text.size(): " << text.size() << " " << endl;
        cout << "\t" << "str.size()/8 : " << str.size()/8 << " " << endl;
        cout << "\t" << "compression ratio : " << float(text.size()) / (str.size() / 8 ) << endl;
        cout << "\t" << "coding price : " << cod_price << endl;
        return make_pair(str, root);
        
	}
	else if (action == 1) {
        if (root == NULL) {
            cout << "����� �� ����������� ";
            return make_pair(" ", nullptr);
        }
		

        if (isLeaf(root)) { while (root->freq--) {  cout << root->ch;  }        }
        else {
            int index = -1;
            while (index < (int)text.size() - 1) {
                 decode(root, index, text, res);
                
            }
        }
        cout << endl;
	}
	return make_pair( res, root);
}

std::string Coder::RLE(bool action, std::string text)
{

    string separator = "_";
	string res;
 


	if (action == 0) { //encode
        
        vector<char> code_rle = encode_RLE(text); 
        

        int count_char = 0;
        
        for (auto i : code_rle) {
            if (i == '\0') { break; }
            bitset<8> x(i);
            res += x.to_string() + separator;
            count_char++;
        }


        string str(code_rle.begin(), code_rle.end());
        return str;
	}
	else if (action == 1) {

		
        res = decode_RLE(text, separator);

	}
	return res;
}

Node* getNode(char ch, int freq, Node* left, Node* right)
{// ������� ��� ��������� ������ ���� ������
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

std::vector<std::string> splitString(const std::string& str, int numParts) {
    std::vector<std::string> result;
    int strLength = str.length();
    int partSize = strLength / numParts;
    int extraSize = strLength % numParts;

    int startPos = 0;
    for (int i = 0; i < numParts; ++i) {
        int currentPartSize = partSize;
        if (i < extraSize)
            currentPartSize++;

        std::string part = str.substr(startPos, currentPartSize);
        result.push_back(part);

        startPos += currentPartSize;
    }

    return result;
}



namespace Operation
{
	bool encoding = false;
	bool decoding = true;
};

int main(int argc, char* argv[])
{
	MPI_Init(&argc,&argv);
	

	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
        
        double starttime,endtime;
        
        
      if(rank == 0){
                
                std::cout<< rank <<  " - rank"<< endl ;
                std::cout<< size <<" - size"<< endl ;
		starttime=MPI_Wtime();
	}
	
	
      std::string alphabet = "abcd";
      int len_text = 1001;
      Coder a;
      
	if(rank == 0){
		generator text("abcde", len_text);
	        string TextInLine =  text.get_string();
	        vector<std::string> TextInVec = splitString(TextInLine, size-1);
		for(int i =1;i<size;i++){
		            std::string txt = TextInVec[i-1];
                            int txtLength = txt.length();
		            std::cout<<"Root send TextInLine("<< txt.substr(0, 10)<<"...) to " << i << "\n";
			   // Отправляем длину строки
                            MPI_Send(&txtLength, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                            // Отправляем саму строку
                            MPI_Send(txt.c_str(), txtLength, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
	}
	else{
                // Получаем длину строки
                int txtLength;
                MPI_Recv(&txtLength, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                // Выделяем буфер для строки
                char* buffer = new char[txtLength + 1];

                // Получаем строку
                MPI_Recv(buffer, txtLength, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                buffer[txtLength] = '\0'; // Добавляем завершающий нулевой символ

                std::string receivedTxt(buffer);
                std::cout<<"rank " << rank << " receive " << receivedTxt.substr(0,20) <<"\n";  // 0 1 2 3
                //ENCODING
		pair<string, Node*> myH = a.Huffman(Operation::encoding, receivedTxt);
	        std::cout << "\tHuffmanCode " << myH.first.substr(0, 100);
	        //DECODING
	        pair<string, Node*> nowH = a.Huffman(Operation::decoding, myH.first);
	        //check
	        std::cout << "\tcheck (encode & decode): " << bool(receivedTxt == nowH.first) << endl<< endl;

	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	
	if(rank == 0){
		endtime = MPI_Wtime();
		std::cout<< "size:"<< size <<"  That took: "<<endtime-starttime<<" secs.\n";
	}
	
	
    
    
    MPI_Finalize();
    return 0;
}
