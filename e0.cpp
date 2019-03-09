#include<iostream>
#include<fstream>
#include<cmath>

using namespace std;

/*void output(char* ordlist, char* file){
   ofstream ordfile;
   ordfile.open(file);
   ordfile << ordlist;
   ordfile.close();
}*/

//0th spot represents length, spots of index greater than result[0] are garbage
int* primes(int n){
   int* result=new int[n];
   result[1]=2;
   int x=3;//current number we're checking primality
   int prime=0;//if the current number seems prime so far
   int k=2;//number of current prime we're working on
   while(x<=n){
      prime=1;
      for(int i=1;result[i]*result[i]<=x;i++){//checks the primes less than the square root of x
         if(!(x%result[i])){
            prime=0;
	    break;
	 }
      }
      if(prime){
	 result[k]=x;
	 k++;
      }
      x+=2;
   }
   result[0]=k-1; //number of primes before k was incremented the last time, prime we're working on is definitely out of range
   return result;
}
//factors is a list of multiplicities which follows ordprimes, first element is index of last element.
int translate(int* factors,int* ordprimes){
   int result=1;
   for(int i=1;i<=factors[0];i++){
      result=result*pow(ordprimes[i],factors[i]);
   }
   return result;
}
//adds the next prime to ordprimes
void newprime(int* primes,int* ordprimes,int** list){
   int j=0;//current index we're looking at in list
   for(int i=1;i<=ordprimes[0];i++){
      do{
         j++;
      }
      while(translate(list[j],ordprimes)>primes[0]);
   }
   ordprimes[ordprimes[0]+1]=primes[translate(list[j],ordprimes)];
   ordprimes[0]+=1;
}

//list is a list of all factorizations in current order, k is the current number that we're looking at in list, n is the maximum number.
void next(int k,int n,int* primes,int* ordprimes,int** list){
   int* result=new int[list[k][0]+1];//same length as before
   result[0]=list[k][0];//creates a new result
   for(int i=1;i<=result[0];i++){
      result[i]=list[k][i];
   }
   for(int i=1;i<=result[0];i++){
      result[i]+=1;
      if(translate(result,ordprimes)>n){
         result[i]=0;
      }
      else break;
   }

   if(!result[result[0]]){//if we've gotten rid of all the factors, make a new prime
      int *newresult=new int[result[0]+2];
      newresult[0]=result[0]+1;
      delete [] result;
      for(int i=1;i<newresult[0];i++){
	 newresult[i]=0;
      }
      newresult[newresult[0]]=1;
      list[k+1]=newresult;
      newprime(primes,ordprimes,list);
   }
   else list[k+1]=result;
}
//factors is formatted by list of multiplicities.
//primes is the list of primes in order, with the 0 spot telling how many.
//remember to delete factors after using to prevent memory leaks.
int* factorize(int c,int* ordprimes){
   int* result=new int[ordprimes[0]+1];
   result[0]=0;
   for(int i=1;i<=ordprimes[0];i++){
      result[i]=0;
      while(!(c%ordprimes[i])){
         c=c/ordprimes[i];
	 result[i]+=1;
	 result[0]=i;//makes it so that result[0] reflects the last prime that actually fits into c.
      }
   }
   return result;
}

int primecounter(int p,int* primes){
   for(int i=1;i<=primes[0];i++){
      if(primes[i]==p){
         return i;
      }
   }
   return 0;//p is not a prime in range.
}

//checks to see if the number has more than one unique prime factor
int needsparenthesis(int p,int* ordprimes){
   int num=0;//number of unique factors
   int* factors=factorize(p,ordprimes);
   for(int i=1;i<=factors[0];i++){
      if(factors[i]){
         num++;
      }
   }
   delete [] factors;
   if(num>1) return 1;
   else return 0;
}

void printordinal(int* factors,int* primes,int* ordprimes){
   for(int i=factors[0];i>0;i--){//largest power of w goes first in the sum.
      if(factors[i]){
	 if(i!=factors[0]){
	    cout << "+";
	 }
         if(factors[i]!=1){
	    cout << factors[i];
	 }
	 if(i!=1&&i!=2){
	    cout << "w^";
            if(needsparenthesis(primecounter(ordprimes[i],primes),ordprimes)){
	       cout << "(";
	    }
	    int* newfactors=factorize(primecounter(ordprimes[i],primes),ordprimes);
	    printordinal(newfactors,primes,ordprimes);
	    delete [] newfactors;
	    if(needsparenthesis(primecounter(ordprimes[i],primes),ordprimes)){
	       cout << ")";
	    }
	 }
	 else if(i==2){
	    cout << "w";
	 }
	 else if(i==1&&factors[i]==1){
            cout << "1";
	 }
      }
   }
   if(!factors[0]){
      cout << "0";
   }
}

//1 means b is bigger, 0 means a is bigger.
/*int compare(int a,int b,int* primes){
   //0 is e0, 1 is 0
   if(a==1||!b){
      return 1;
   }
   if(b==1||!a){
      return 0;
   }
   int* afactors=new int[a];
   int* bfactors=new int[b];
   factorize(a,afactors,primes);
   factorize(b,bfactors,primes);*/

int main(){
   int n;
   cout << "Enter a number" << endl;
   cin >> n;
   int* p=primes(n);
   int* ordprimes=new int[p[0]+1];
   ordprimes[0]=0;//make sure ordprimes knows there's garbage in it.
   int** list=new int*[n];
   list[0]=new int[1];//add 1 to the list to start off
   list[0][0]=0;//believe it or not, this translates to 1.
   for(int k=0;k<n-1;k++){
      next(k,n,p,ordprimes,list);
   }

   //print out list
   for(int i=0;i<n;i++){
      printordinal(list[i],p,ordprimes);
      cout << endl;
   }
   
   //free memory
   delete [] p;
   delete [] ordprimes;
   for(int i=0;i<n;i++){
      delete [] list[i];
   }
   delete [] list;

   return 0;
}
