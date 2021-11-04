#include <stdio.h>
#include "Graph.h"
#include <stdlib.h>
#include <string.h>

TGraphL* createGraphAdjList(int numberOfNodes) 
{
	TGraphL *TL;
	TL = malloc( sizeof( TGraphL ) );
	TL -> nn = numberOfNodes ;
	TL -> adl = malloc( numberOfNodes * sizeof( ATNode ) );
	for( int i = 0 ; i < numberOfNodes ; i ++ )
		TL -> adl[ i ] = NULL ;
	return TL;
}

void addEdgeList( TGraphL* graph, int v1, int v2) 
{   
    //adaugam v2 la v1
	ATNode n1  ;
	n1 = malloc( sizeof( TNode ) );
	n1 -> v = v1 ;
	n1 -> next = NULL ;
	
	n1 -> next = graph -> adl[ v2 ] ;
	graph -> adl[ v2 ] = n1 ;
	
}

int find_node( char*name[ 100 ]  , int *n , char *s )
{
    int i ;
    for( i = 0 ; i < *n ; i ++ )
        if( strcmp( name[ i ] , s ) == 0 )
            return i ;
    return -1;
}

void destroyGraphAdjList(TGraphL* graph)
{
	int j ;	
	ATNode i , aux  ;
	for( j = 0 ; j < graph -> nn ; j ++ )
		{
			i = graph -> adl[ j ];
			while( i != NULL )
			{
				aux = i ;
				i = i -> next ;
				free( aux );
			}
		}
	free( graph -> adl );
	free( graph );
}

TGraphL* read_graph( int *n , int *m , char *name[ 100 ] , FILE *in  )
{
    TGraphL *G ;
    
    int i , pnr , fnr ;
    char *p , *f;
    p = malloc( 10 * sizeof( char ) );
    f = malloc( 10 * sizeof( char ) );
    fscanf( in , "%d %d" , n , m );
    //printf( "%d\n" ,  );
    for( i = 0 ; i < *n ; i ++ )
        {   
            name[ i ] = malloc( 10 * sizeof( char) );
            fscanf( in , "%s" , name[ i ] );
        }
    G = createGraphAdjList( *n );
    for( i = 0 ; i < *m ; i ++ )
    {
        fscanf( in , "%s %s" , p , f );
        pnr = find_node( name , n , p );
        fnr = find_node( name , n , f ); 
        //printf( "%d %d\n", pnr , fnr );
        addEdgeList( G , fnr , pnr );
    }
    free( p );
    free( f );
    return G;
    
}

int cycle = 0 ; 

void DFS( int node , int viz[]  , TGraphL *G )
{   
    ATNode j  ;
    //printf( "%d \n" , node );
    if( viz[ node ] == 0 )
        {
            viz[ node ] = 1 ;
            
            for(j = G -> adl[ node ] ; j != NULL ; j = j -> next )
            {   //printf("%d %d\n" , node , j -> v );
                if( viz[ j -> v ] == 0 ) 
                    DFS( j -> v , viz , G );
                
                if( viz[ j -> v ] == 1 )
                        cycle = 1 ;
            }
        
        }
        viz[ node ] = 0 ;
        
}

int isCyclic( TGraphL *G )
{
    int *viz  , i ;
    viz = calloc( G -> nn + 1 , sizeof( int ) );
    for( i = 0 ; i < G -> nn; i++ )
    {
        viz[ i ] = 0 ;
    }
    for( i = 0 ; i < G -> nn && cycle == 0  ; i++ )
        DFS( i , viz  , G ) ;
            
    free( viz );
    return cycle ;
}

void DoTask1()
{
    FILE *in , *out ;
    TGraphL *G ;
    int n , m , rez ;
    char *name[  100 ];
    in = fopen( "bnet.in" , "r" );
    out = fopen( "bnet.out" , "w" );
    G = read_graph( &n , &m , name , in );
    rez = isCyclic( G );
    
    if( rez == 1 )
           fprintf( out , "imposibil\n");
   if( rez == 0 )
            fprintf( out , "corect\n");         
   for(int i = 0 ; i < G -> nn ; i++ )
        free( name[ i ] );
   destroyGraphAdjList( G );   
}

TGraphL* reverse( TGraphL* G )
{
    TGraphL* G_rev ;
    G_rev = createGraphAdjList( G -> nn  );
    TNode *j ;
    int i ;
    for( i = 0 ; i < G -> nn ; i++ )
    {
        j = G -> adl[ i ];
        while( j != NULL )
        {   
            addEdgeList( G_rev , i , j -> v );
            j = j -> next ;
        }
    }
    return G_rev ;
      
}

void anc( int node , int viz[]  , TGraphL *G )
{   
    viz[ node ] = 1 ;
    ATNode j  ;
    //printf( "%d \n" , node );
    for(j = G -> adl[ node ] ; j != NULL ; j = j -> next )
    {   //printf("%d %d\n" , node , j -> v );
        if( viz[ j -> v ] == 0 ) 
            anc( j -> v , viz , G );
    }
        
}

void print_graf( TGraphL *G )
{
    int i ;
    ATNode j ;
    for( i = 0 ; i < G -> nn ; i ++ )
    {
        printf( "%d   : " , i );
        for(j = G -> adl[ i ] ; j != NULL ; j = j -> next )
            printf( "%d " , j -> v );
        printf( "\n" );
    }
}

void moralizare( TGraphL *G , int anc[] , int ad[ 20 ][ 20 ] )
{
    int i , muchii[ 50 ][ 2 ] , ct = 0 ;
    ATNode j , k  ;
    for( i = 0 ; i < G -> nn ; i++ )
        if( anc[ i ] == 1 )
            for( j = G -> adl[ i ] ; j != NULL ; j = j -> next )
                for( k = j -> next ; k != NULL ; k = k -> next )
                    {
                        muchii[ ct ][ 0 ] = j -> v ;
                        muchii[ ct ][ 1 ] = k -> v ;
                        ct ++ ;
                    }
    //cu toate ca adaugam o mmuchie de mai multe ori nu afeacteaza cu ceva algoritmul
    for( i = 0 ; i < ct ; i ++ )
        {   
            if( ad[ muchii[ i ][ 1 ] ][ muchii[i][ 0 ] ] == 0 )
                {
                    addEdgeList( G , muchii[i][ 0 ] , muchii[ i ][ 1 ] );
                    ad[ muchii[ i ][ 1 ] ][ muchii[i][ 0 ] ] = 1 ;
                }
            if( ad[ muchii[ i ][ 0 ] ][ muchii[i][ 1 ] ] == 0)
                {
                    addEdgeList( G ,  muchii[ i ][ 1 ] , muchii[i][ 0 ] );
                    ad[ muchii[ i ][ 0 ] ][ muchii[i][ 1 ] ] = 1 ;
                }
        }
}

void neorientat( TGraphL *G  , int ad[ 20 ][ 20 ] )
{
    int i ;
    ATNode j   ;
    for( i = 0 ; i < G -> nn ; i++ )
        for( j = G -> adl[ i ] ; j != NULL ; j = j -> next )
            {
                // daca nu exista muchia complementara o adaugam
                if( ad[ j -> v ][ i ] == 0 )
                    {
                        addEdgeList( G , i , j -> v );
                        ad[ j -> v ][ i ] = 1 ;
                    }

            }
    
}

void create_ad_matrix( TGraphL *G , int mat[ 20 ][ 20 ] )
{
    TNode *j ;
    int i ;
    for( i = 0 ; i < G -> nn ; i++ )
    {
        j = G -> adl[ i ];
        while( j != NULL )
        {   
            mat[ i ][ j -> v ] = 1 ;
            j = j -> next ;
        }
    }
}

void reach_distance( int node , TGraphL *G , int vis[ 20 ] , int ance[ 20 ] )
{
    vis[ node ] = 1 ;
    
    ATNode j  ;
    for(j = G -> adl[ node ] ; j != NULL ; j = j -> next )
    {   //printf( "%d %d\n" , j -> v , ance[ j -> v ] );
        if( vis[ j -> v ] == 0 && ance[ j -> v ] == 1 ) 
            reach_distance( j -> v, G , vis , ance );
    }
    
}

void independenta( FILE *in , TGraphL *G , char*name[ 100 ] , FILE *out )
{
    char s[ 100 ] , *p , nodes[ 20 ][ 20 ] = { 0 } ;
    int ct = 0 , nr_nodes[ 20 ] , ance[ 20 ]={ 0 } , ad[ 20 ][ 20 ] ;  
    fgets( &s, 100 , in );
    p = strtok( s , " |;\n" );
    //programul citeste o linie goala la inceput
    if( p == NULL )
        return ;
    while( p != NULL )
    {  
        ct ++ ;
        strcpy( nodes[ ct - 1 ] , p );
        //printf( "%s\n" , p );
        p = strtok( NULL , " |;\n" ) ;
    }
    TGraphL *G_rev ; // graful intors 
    G_rev = reverse( G );
    
    //cream graful ancestral
    //ance[ i ] = 1 daca nodul face parte din graful ancestral
    for(int i = 0 ; i < ct ; i++ )
    {   
        nr_nodes[ i ] = find_node( name , &G -> nn  , nodes[ i ] );
        if( ance[ nr_nodes[ i ] ] == 0 )
        {
            anc( nr_nodes[ i ] , ance , G_rev );
        }
    } 
    
    //cream o matrice de adiacenta pentru a verifica existenta nodurilor in O(1)
    //dar si pentru a verifica daca trebuie sa adaugam complementarul
     for( int x = 0 ; x < G -> nn ; x ++ )
            for( int y = 0 ; y < G -> nn ; y ++ )
               ad[ x ][ y ] = 0 ;
    create_ad_matrix( G_rev , ad );

    // moralizam graful format din nodurile care au ance = 1 
    moralizare( G_rev , ance , ad );
    
    //transformam graful moralizat intr-un graf normal
    neorientat( G_rev , ad );
   
    
    //scoatem din ance nodurile care nu ne intereseaza
    for(int i = 2 ; i < ct ; i++ )
    {   
        ance[ nr_nodes[ i ] ] = 0 ;
    } 
    //cautam un drum intre a si b 
    int reach[ 20 ] = { 0 } ;   
    reach_distance( nr_nodes[ 0 ] , G_rev , reach , ance );

    if( reach[ nr_nodes[ 1 ] ] == 0 || ance[ nr_nodes[ 0 ] ] == 0 || ance[ nr_nodes[ 1 ] ] == 0 )
        fprintf( out , "independente\n" );
    else
        fprintf( out , "neindependente\n" );
    destroyGraphAdjList( G_rev );
}

void DoTask2()
{
  FILE *in , *out ;
    TGraphL *G ;
    int n , m  , nr_quests , k ;
    char *name[  100 ];
    in = fopen( "bnet.in" , "r" );
    out = fopen( "bnet.out" , "w" );
    G = read_graph( &n , &m , name , in );
    fscanf( in , "%d" , &nr_quests );
    for( k = 1 ; k <= nr_quests + 1 ; k++ )
    {
      independenta( in , G , name , out );  
    }
    for(int i = 0 ; i < G -> nn ; i++ )
        free( name[ i ] );
     destroyGraphAdjList( G );
}


int main(int argc, char *argv[])
{
    //printf( "%s\n" , argv[ 1 ] );
    if( strcmp( argv[ 1 ] , "-c1" ) == 0 )
        DoTask1();
    if( strcmp( argv[ 1 ] , "-c2" ) == 0 )
        DoTask2();
    return 0;
}