#pragma warning(disable:4786) 

#ifndef hmap_h
#define hmap_h

#include <vector>
#include "incl.h"

using namespace std;

// void insert( obj_type x )       --> Insert x
// void remove( key_type x )       --> Remove x
// obj_type find( key_type x )  --> Return item that matches x
// void makeEmpty( )      --> Remove all items

template <class obj_type, class key_type>
class hmap
{
private:
 enum entry_type 
 { 
  ACTIVE, EMPTY, DELETED
 };

 struct hash_entry
 {
  obj_type   element;
  entry_type info;

  hash_entry( const obj_type & e = obj_type( ), entry_type i = EMPTY ) : element( e ), info( i ) { }
 };
    
 int occupied;
	
 virtual bool isActive( int currentPos ) const;
 virtual void rehash( );
 virtual bool isPrime  ( int n ) const;
 virtual int  nextPrime( int n ) const;
 double maxOccupiedPercentage; 

protected:
 int lookups;
 unsigned int hash( const string & key ) const;
 vector<hash_entry> array;

public:
 hmap(double moc );

 virtual int  findPos  ( const key_type &x );
 virtual void makeEmpty( );
 virtual void insert   ( const obj_type &x );
 virtual void remove   ( const key_type &x );
 virtual obj_type* find( const key_type &x );


 // inline:
 int getSize() 
 {
  int size = 0;
  for( int j = 0; j < array.size( ); j++ )
   if (array[ j ].info == ACTIVE)
    size++;
   return size;	
 };

 int getLookups()
 {
  return lookups;
 };

 int getCapacity()
 {
  return array.size();
 };

 double getLambda()
 {
  return static_cast<double>(getSize())/static_cast<double>(getCapacity());
 }

};

template <class obj_type, class key_type>
class linearhmap : public hmap<obj_type, key_type> {
public:
 linearhmap(double moc) : hmap<obj_type, key_type>(moc) {};

 virtual int findPos( const obj_type & x )
 {
  int collisionNum = 0;
  int currentPos = hash( x ) % array.size( );
  lookups++;

  while( array[ currentPos ].info    != EMPTY &&
         array[ currentPos ].element != x )
  {	
   lookups   ++;
   currentPos++; 

   if( currentPos >= array.size( ) )
    currentPos -= array.size( );
  }

  return currentPos;
 }
};

#include "hmap.cpp"

#endif
