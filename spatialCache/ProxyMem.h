#ifndef PROXYMEM_H
#define PROXYMEM_H

#include <list>
#include "WPoint.h"
#include <fstream>

using namespace std;

struct int3 {
	int x,y,z;

	int3(const int& a, const int& b, const int& c){
		x = a;
		y = b;
		z = c;
	}
};

typedef list<WPoint*> PointList; //TODO pas bien en double dans wworld

class ProxyMem{
	private:
	//ofstream logfile;
	list<WPoint*> *table;
	long	*timestamp;
	int		*group;
	public:
	int m_pointCount;
	unsigned int m_size;
	real m_cellsize, m_invcellsize;
	
	ProxyMem(real cellsize, int size);
	~ProxyMem();
	void			addPoint(WPoint *p);
	//void			update(const PointList& list);
	void			update();
	void			update(WPoint **ptab,int size, long tstamp);
	unsigned int	hash( const WPoint& p);
	unsigned int	hash( const int &x, const int &y, const int &z);

	int3			discretize( const float3& p);
	float3			getCellCenter(const int &x, const int &y, const int &z);
	int				getPopulation(const int &x, const int &y, const int &z);

	list<WPoint*>&  getCell(const int3& adr);
	list<WPoint*>&  getCell(unsigned int hash);
	list<WPoint*>&	getCell(const int &x, const int &y, const int &z);
	bool			IsInCell(const WPoint& p, const int &x, const int &y, const int &z);
	
	unsigned int	Size(){
		return m_size;
	};

	inline long		timeStamp(unsigned int h);

	
};

inline void ProxyMem::addPoint(WPoint *p){
	//unsigned int h = hash(*p);
	table[hash(*p)].push_back(p);
	m_pointCount++;
}


inline unsigned int ProxyMem::hash( const WPoint& p){
	const int x = (int) floor(p.p.x*m_invcellsize);
	const int y = (int) floor(p.p.y*m_invcellsize);
	const int z = (int) floor(p.p.z*m_invcellsize);
	return hash(x,y,z);
}

inline unsigned int ProxyMem::hash(const int &x, const int &y, const int &z){
	return ( (x*73856093) ^ (y*19349663) ^ (z*83492791) )%m_size;
	//return ( ((x+10)*73825) * ((y+10)*91434) * ((z+10)*153234) ) % m_size;
}

	
inline int3 ProxyMem::discretize( const float3& p){
	return int3((int) floor(p.x*m_invcellsize),
				(int) floor(p.y*m_invcellsize),
				(int) floor(p.z*m_invcellsize));
}

inline float3 ProxyMem::getCellCenter(const int &x, const int &y, const int &z){
	return float3(((real)x+0.5f) * m_cellsize,
				  ((real)y+0.5f) * m_cellsize,
				  ((real)z+0.5f) * m_cellsize );
}

inline bool ProxyMem::IsInCell(const WPoint& p, const int &x, const int &y, const int &z){
	const int rx = (int) floor(p.p.x*m_invcellsize);
	const int ry = (int) floor(p.p.y*m_invcellsize);
	const int rz = (int) floor(p.p.z*m_invcellsize);
	return ((rx==x)&&(ry==y)&&(rz==z));
}

inline list<WPoint*>& ProxyMem::getCell(const int &x, const int &y, const int &z){
	return table[hash(x,y,z)];
}

inline list<WPoint*>& ProxyMem::getCell(const int3& adr){
	return table[hash(adr.x,adr.y,adr.z)];
}

inline list<WPoint*>& ProxyMem::getCell(unsigned int h){
	return table[h];
}

inline long ProxyMem::timeStamp(unsigned int h){
	return timestamp[h];
}


#endif
	