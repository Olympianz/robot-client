#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
  long data;
  int count = 0;
  ifstream myData ("test.txt");
  ofstream plotData;
  plotData.open ("gnuplotData.txt");

  if ( myData.is_open() ){
    while( myData >> data ){
      if (count == 0){ 
	plotData << data;
	plotData << "  ";
        cout << endl << "Show me the data: " << data;	
      }
    myData.clear();
      /*
      if (count == 7){
	plotData << data;
	plotData << endl;
      }

      ++count;
      if (count == 11)
	count = 0;
	*/
    }
  }

  plotData.close();
  return 0;
}
