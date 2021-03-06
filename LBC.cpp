// LBC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"    //exclude if you are not using visual studio
#include<iostream>
#include<stdio.h>
#include<math.h>

using namespace std;


//class lbc containing necessary elements 

class lbc
{
public:
	lbc(int, int, int, int);
	~lbc();
	void input();
	void checkType();
	void generateMatrices();
	void generateCodeVectors();
	void decode();
	

private:
	int n, k, x, y,dMin,s,ts,power;
	bool **ipMatrix, **genMatrix, **hMatrix, **pMatrix, **symbols,***checkBits,**checkBitMatrix, **codeVectors,**syndromeMatrix,**hTMatrix;
	int **noOfOnes, *weight;
	char type;
	void print(bool**, int, int);
	void print(int**, int, int);
	void printCodeVectorTable();
	void capacity();
	void initDecode();

	
};


// constructor for class lbc

lbc::lbc(int n1, int k1, int x1, int y1)
{
	ipMatrix = new bool*[x1];
	for (int i = 0; i < x1; ++i)   ipMatrix[i] = new bool[y1];
		
	n = n1;
	k = k1;
	x = x1;
	y = y1;
	power = pow(2, k);

}

// a function take a matrix of defined order from user

void lbc::input()
{
	cout << "enter elements\n";
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			cout << "(" << i << "," << j << ") \t";
			cin >> ipMatrix[i][j];
		} cout << "\n";
	}
}




void lbc::print(bool **mat, int row, int col)
{ 
	for (int i = 0; i < row; i++)
	{
		cout << "\t";
		for (int j = 0; j < col; j++)
		{
			//cout << bitset<8>(ipMatrix[i * j]) << "\t";
			cout << mat[i][j] << "\t";
		} cout << "\n";


	}
}

void lbc::print(int **mat, int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		cout << "\t";
		for (int j = 0; j < col; j++)
		{
			//cout << bitset<8>(ipMatrix[i * j]) << "\t";
			cout << mat[i][j] << "\t";
		} cout << "\n";


	}
}

void lbc::printCodeVectorTable()
{
	cout << "\n\t Message bits \t | \t Check Bits \t | \t Code Vectors\n";
	cout << "\n\t------------------------------------------------------------------\n";
	for (int i = 0; i < pow(2, k); i++)
	{
		cout << "\t \t";
		for (int j = 0; j < k; j++) cout << symbols[i][j];
		cout << "\t | \t";
		for (int j = 0; j < n - k; j++) cout << checkBitMatrix[i][j];
		cout << "\t\t | \t     ";
		for (int j = 0; j < n; j++) cout << codeVectors[i][j];
		cout << "\n";
		
	} cout << "\n";
}


// a function to check the type of matrix entered by user

void lbc::checkType()
{
	if (x == k && y == (n - k))                    // checks if entered matrix is a parity bit matrix
	{
		type = 'P';             
		cout << "\n the entered is a parity bit matix";
		return;
	}


	if (x == k && y == n)                         // checks if entered matrix is generator matrix
	{
		bool flag= true;
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < (y - x); j++)
			{
				if (i != j && ipMatrix[i][j] != 0)
				{   
					flag = false;
					break;
				}

				if (i == j && ipMatrix[i][ j] != 1)
				{   
					flag = false;
				    break;
                }


			}
		}

		if (flag)
		{
			type = 'G';             
			cout << "\n the entered is a generator matix";
			return;
			
		}
	}


	if (x == n-k && y == n)                         // checks if entered matrix is hamming matrix
	{
		int diff = y - x;
		bool flag =true;
		for (int i = 0; i < x; i++)
		{
			for (int j = (y-x); j < y; j++)
			{
				if ((i+j == diff) && ipMatrix[i][j] != 1)
				{
					flag = false;
					break;
				} 
				if ( (i+j)!=diff && ipMatrix[i][j] != 0)
				{
					flag = false;
					break;
				}


			}  diff += 2;
		}

		if (flag)
		{
			type = 'H';
			cout << "\n the entered is a hamming matix";
			return;
			
		}
	}

}


// a function to generate H, G and P

void lbc::generateMatrices()
{
	cout << "\n...............................Generating Matrices .............................\n";
	
	genMatrix = new bool*[k];
	for (int i = 0; i < k; ++i)   genMatrix[i] = new bool[n];

	hMatrix = new bool*[n-k];
	for (int i = 0; i < (n-k); ++i)   hMatrix[i] = new bool[n];

	pMatrix = new bool*[k];
	for (int i = 0; i < k; ++i)   pMatrix[i] = new bool[n-k];


	switch (type)
	{
	case 'P' :
	{   
		pMatrix = ipMatrix;

		cout << "\n Parity Matrix  : \n";
		print(ipMatrix, k, n - k);

		//**********************************************************generator matrix  remember G=[I:P]

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j <= (n - k); j++)
			{
				if (i != j)  genMatrix[i][j] = 0;
				else if (i == j) genMatrix[i][j] = 1;				
			}
		}

		
		for (int i = 0; i < k; i++)
		{
			for (int j = (n - k)+1, p=0; j < n && p<(n-k) ; j++,p++)
			{
				genMatrix[i][j] = pMatrix[i][p];
			}  
		}

		cout << "\n Generator Matrix Generated : \n";
		print(genMatrix, k, n);


		//**********************************************************Hamming Matrix  remember H=[Pt:I]

	
		int diff = k;

		for (int i = 0; i < (n - k); i++)
		{
			for (int j = 0; j < k; j++)
			{
				hMatrix[i][j] = pMatrix[j][i];

			}
		}

		for (int i = 0; i < (n - k); i++)
		{
			for (int j = k; j < n; j++)
			{
				if ((i + j == diff)) hMatrix[i][j] = 1;
				
				else if ((i + j) != diff) hMatrix[i][j] = 0;
				


			}  diff += 2;
			
		}

		cout << "\n Hamming Matrix Generated : \n";
		print(hMatrix, n-k, n);

		break;

	}

	case 'G' :
	{
		genMatrix = ipMatrix;
		cout << "\n Generator Matrix  : \n";
		print(genMatrix, k, n );

		//**************************************************** Parity Matrix remember G=[I:P]

		for (int i = 0; i < k; i++)
		{
			for (int j = ((n - k) + 1),p=0; j < n,p<(n-k); j++,p++)
			{
				pMatrix[i][p] = genMatrix[i][j];
			}
		}
         
		cout << "\n Parity Matrix Generated  : \n";
		print(pMatrix, k, n - k);


		//*************************************************** Hamming Matrix remember H=[Pt:I]

		for (int i = 0; i < n - k; i++)
		{
			for (int j = 0; j < k; j++)
			{
				hMatrix[i][j] = pMatrix[j][i];

			}
		}

		int diff = k;
		for (int i = 0; i < (n - k); i++)
		{
			for (int j = k; j < n; j++)
			{
				if ((i + j == diff)) hMatrix[i][j] = 1;

				else if ((i + j) != diff) hMatrix[i][j] = 0;

			}  diff += 2;

		}

		cout << "\n Hamming Matrix Generated  : \n";
		print(hMatrix, n-k, n);

		break;


	}

	case 'H':
	{
		hMatrix = ipMatrix;
		cout << "\n Hamming Matrix :\n";
		print(hMatrix, n - k, n);

		//*********************************************generating parity matrix remember H=[pt:I]

		for (int i = 0; i < k;i++)
		{
			for (int j = 0; j < n-k; j++)
			{
				pMatrix[i][j] = hMatrix[j][i];
			}
		}

		cout << "\n Parity Matrix generated :\n";
		print(pMatrix, k, n - k);

		//**********************************************************generator matrix  remember G=[I:P]

		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j <= (n - k); j++)
			{
				if (i != j)  genMatrix[i][j] = 0;
				else if (i == j) genMatrix[i][j] = 1;
			}
		}


		for (int i = 0; i < k; i++)
		{
			for (int j = (n - k) + 1, p = 0; j < n && p<(n - k); j++, p++)
			{
				genMatrix[i][j] = pMatrix[i][p];
			}
		}

		cout << "\n Generator Matrix Generated : \n";
		print(genMatrix, k, n);
		
	}

	
	}
}


// a function to generate code vectors

void lbc::generateCodeVectors()
{   

	// generate a matrix of binary message bits from 0 to 2^k

	

	symbols = new bool*[power];
	for (int i = 0; i < power; ++i)  symbols[i] = new bool[k];

	int count ;
	bool bit ;

	for (int bitPos = 0; bitPos < k; bitPos++)
	{
		count = 0;
		bit = 0;
		
			for (int j = 0; j < power; j++)
			{
				symbols[j][k-(bitPos+1)] = bit;
				++count;
				if (count == pow(2,bitPos))
				{   count = 0;
					if (bit)
					{
						bit = 0;
					}
					else
					{
						bit = 1;
					}


				}
			}
			
		 
	}

	cout << "\nwe have following message bits : \n";
	print(symbols, power, k);

	// generate check bit matrix for each bit (c1, c2, c3,....) for ex-oring i.e (n-k) matrices of order (power * k)
    
	checkBits = new bool**[n - k];
	for (int i = 0; i < (n - k); ++i)
	{
		checkBits[i] = new bool*[power];
		for (int j = 0; j < power ; j++)
		checkBits[i][j] = new bool[k];

	   }

	for (int i = 0; i < n - k ; i++)
	{
		for (int j = 0; j < power; j++)
		{
			for (int l = 0; l < k; l++)
			{
				checkBits[i][j][l] = symbols[j][l] * pMatrix[l][i];
			}
		}
	}


	/*cout << "\nmultiplying check bits with respective message bits  :";
	for (int i = 0; i < n - k; i++)
	{
		print(checkBits[i], power, k);
		cout << "\n";
	}*/


	// now that checkbits have been multiplied with corresponding message bits, thing left to do is moduli-2 addition of check bits

	noOfOnes = new int*[power];
	for (int i = 0; i < power; i++) noOfOnes[i] = new int[n - k];
	
	for (int i = 0; i < power; i++)
		for (int j = 0; j < n-k; j++)
			noOfOnes[i][j] = 0;                                                    //count no of ones in checkbits

	for (int i = 0; i < n - k; i++)
	{
		for (int j = 0; j < power; j++)
		{
			for (int l = 0; l < k; l++)
			{
				if (checkBits[i][j][l] == 1) noOfOnes[j][i]++;
				
			}
		}
	}

	

	//*************** moduli 2  on the basis of logic that if a there are odd number of ones then op is 1 otherwise 0

	checkBitMatrix = new bool*[power];
		for (int i = 0; i < power; i++) checkBitMatrix[i] = new bool[n - k];
    
		for (int i = 0; i < power; i++)
		{
			for (int j = 0; j < k; j++)
			{
				if (noOfOnes[i][j] % 2 == 0)
				{
					checkBitMatrix[i][j] = 0;
				}
				else
				{
					checkBitMatrix[i][j] = 1;
				}
			}
		}
    
    //****************** creating code vector matrix 

		codeVectors = new bool*[power];
		for (int i = 0; i < power; i++) codeVectors[i] = new bool[n];

		for (int i = 0; i < power; i++)
		{
			for (int j = 0; j < k; j++)
				codeVectors[i][j] = symbols[i][j];

			for (int j = k; j < n; j++)
				codeVectors[i][j] = checkBitMatrix[i][j-k];
		}
       
		cout << "\n code vectors :";
	//	print(codeVectors, power, n);
		printCodeVectorTable();
		capacity();
		

	
}

// a fuunction to calculate the channel capacity

void lbc::capacity()
{
	//****************************** calculate weight of each code vector
	weight = new int[power];
	for (int i = 0; i < power; i++)
	{
		weight[i] = 0;
		for (int j = 0; j < n; j++) 
			if(codeVectors[i][j]) 
				weight[i]++;
	}

	//************************************* find dMin
	dMin = weight[1];
	for (int i = 2; i < power; i++) if (weight[i] < dMin) dMin = weight[i];

	//************************************calculate s and ts
	s = dMin - 1;
	ts = s / 2;

	//************** print the capacity of code

	cout << " \n \there we have dmin =" << dMin << "\t therefore  the code can detect   " << s << "  errors and correct   " << ts << "  of them\n";
}

void lbc::initDecode()
{
	    cout << "\n\n...............................Generating Error Vector Matrix..............................";

	    syndromeMatrix = new bool*[n+1];		
		for (int i = 0; i <= n; i++) syndromeMatrix[i] = new bool[n-k];

		hTMatrix = new bool*[n];
		for (int i = 0; i < n; i++) hTMatrix[i] = new bool[n - k];
    
		for (int i = 0; i < n - k; i++) syndromeMatrix[0][i] = 0;
		
		for (int i = 1,l=0; i <= n,l<n; i++,l++)
		{
			for (int j = 0; j < n - k; j++)
			{
				syndromeMatrix[i][j] = hMatrix[j][l];
				hTMatrix[l][j] = hMatrix[j][l];
			}
		}


		cout << "\n\nGenerated Error Vector Matrix : \n\n";
		print(syndromeMatrix, n+1, n - k);


}

void lbc::decode()
{
	initDecode();

	char choice='y';

	char *ipVector = new char[n];
	
	bool *vector = new bool[n];
	
	bool **syndromeMul = new bool*[n];
	
	bool *syndrome = new bool[n - k];
	for (int i = 0; i < n; i++) syndromeMul[i] = new bool[n - k];
	
	int *countOne = new int[n-k];

		
	while (choice == 'y' || choice == 'Y') {
		cout << "\n\n*************************************************************************************************\n enter the receieved vector (" << n << " bits long) you want to decode/correct  \t";
		cin >> ipVector; cout << endl;
		for (int i = 0; i < n; i++)
		{
			if (ipVector[i] == '1') vector[i] = 1;
			else vector[i] = 0;
		}
		
		for (int i = 0; i < n - k; i++) countOne[i] = 0;
		
		for (int i = 0; i < n; i++)
		{
			
			for (int j = 0; j < n-k; j++)
			{
				syndromeMul[i][j] = vector[i] * hTMatrix[i][j];

				if (syndromeMul[i][j] == 1) countOne[j]++;
			}
		    
		}

		for (int i = 0; i < n - k; i++)
		{
			if (countOne[i] == 1) syndrome[i] = 1;
			if (countOne[i] % 2 == 0) syndrome[i] = 0;
			else syndrome[i] = 1;
		}

		cout << "\n on multiplying vector and Ht we get syndrome :\t";
		for (int i = 0; i < n - k; i++) cout << syndrome[i];
          

		cout << endl << "\n\n............................ checking Error Vector Matrix for syndrome.............................\n ";
		
		bool flag;
		int position;
		for (int i = 0; i < n + 1; i++)
		{
			position = i;
			flag = true;
			for (int j = 0; j < n-k; j++)
			{
				if (syndromeMatrix[i][j] != syndrome[j]) flag = false;
			}
			if (flag) break;
		}

		

		cout << "\n found the syndrome at position " << position <<" in Error Vector table \n ";
		 
		

		if (position == 0) cout << "\n there is no error in the receieved codeword " << "\n\n ...................searching for recieved code word in code vector table....................... ";
		else 
		{
			if (vector[position - 1] == 1)
			{
				vector[position - 1] = 0;
			}
			else 
			{
				vector[position - 1] = 1;
			}

			cout << "\n recieved vector corrected to : ";
			for (int i = 0; i < n; i++) cout << vector[i];

			
			cout << "\n\n ...................searching for recieved code word in code vector table....................... ";

		}

		for (int i = 0; i < power; i++)
		{
			flag = true;
			for (int j = 0; j < n; j++)
			{
				if (vector[j] != codeVectors[i][j]) flag = false;
			}
			if (flag)
			{
				position = i;
				break;
			}

			
		}
		cout << "\n\n received codeword found and can be looked up in code vector table at position "<<position+1<<endl;
		cout << "\n\n --------------------DO YOU WANT TO CHECK DECODE/CORRECT ANOTHER : ";
		cin >> choice;

	} 
}

lbc::~lbc()
{
	
}

//*************************************** Function Main
int main()
{

	int n, k, x, y;     //where variables n & k  have there usual meaning as in LBC and x & y are order of matrix user has
	char chc;
	
	cout << "\t_______________________________ LBC Encoder And Decoder_______________________________\n";
	cout << "\nenter the order of lbc (n,k), first n then k";
	cin >> n >> k;
	cout << "\n**************************************************************************************\n";
	cout << "enter the order of matrix you want to enter (x*y), first x then y";
	cin >> x >> y;
	cout << "\n**************************************************************************************\n";
	lbc container(n,k,x,y);

	container.input();
	container.checkType();
	container.generateMatrices();
	container.generateCodeVectors();
	
	cout << "\n Do you wish to proceed for Decoding ? (Y|N) \t";
	cin >> chc;

	if (chc == 'y' || chc == 'Y') container.decode();
	
	return 0;
}


