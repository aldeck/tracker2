#include "ProxyMem.h"
#include <fstream>
#include "math/geotools.h"

ProxyMem::ProxyMem(real cellsize, int size){
	table = new list<WPoint*> [size];
	timestamp = new long [size];
	group = new int [size];
	for(int i=0;i<size;i++) {
		timestamp[i]=0;
		group[i]=0;
	}
	m_pointCount = 0;
	m_size = size;
	m_cellsize = cellsize;
	m_invcellsize = 1.0 / m_cellsize;

}

ProxyMem::~ProxyMem(){

}

/*void ProxyMem::addPoint(WPoint *p, long stamp){
	
	unsigned int h = hash(*p);
	if (timestamp[h]!=stamp){ //out of date
		table[h].clear();
		timestamp[h]=stamp;
		table[h].push_back(p);
	} else {
		table[h].push_back(p);
	//m_pointCount++;
	}
}*/

/*void ProxyMem::update(const PointList& list){
	for(int cell=0; cell<m_size; cell++){
		list<WPoint*>& plist = table[cell]; 
		list<WPoint*>::iterator p = plist.begin();
		while (p!=plist.end()){
			unsigned int h = hash(**p);

			//TODO expe
			(*p)->state = 0;  //Pourrait etre ailleurs

			if ((h != cell)){
				table[h].push_back(*p); 
				p = plist.erase(p);
			} 
			p++;			
		}			
	}
}*/

void ProxyMem::update(){
	for(int cell=0; cell<m_size; cell++){
		list<WPoint*>& plist = table[cell]; 
		list<WPoint*>::iterator p = plist.begin();
		while (p!=plist.end()){
			unsigned int h = hash(**p);

			//TODO expe
			(*p)->state = 0;  //Pourrait etre ailleurs

			if ((h != cell)){
				table[h].push_back(*p); 
				p = plist.erase(p);
			} 
			p++;			
		}			
	}
}

void ProxyMem::update(WPoint **ptab,int size, long tstamp){//pas bien
	
	for(int i=0;i<size;i++){
		WPoint * p = ptab[i];
		
		unsigned int h = hash(*p);

		//TODO expe
		p->state = 0;  //Pourrait etre ailleurs

		if (timestamp[h]!=tstamp){
			timestamp[h]=tstamp;
			table[h].clear();			
		}

		table[h].push_back(p); 					
	}			
}

int ProxyMem::getPopulation(const int &x, const int &y, const int &z){
	return getCell(x,y,z).size();
}
	
