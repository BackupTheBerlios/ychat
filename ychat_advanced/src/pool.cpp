#ifndef POOL_CPP
#define POOL_CPP

#include "pool.h"
#include "thrd.h"

using namespace std;

pool::pool()
{
    i_thrd_pool_size  = tool::string2int( wrap::CONF->get_elem( "THREAD_POOL_SIZE" ) );
    i_thrd_pool_queue = tool::string2int( wrap::CONF->get_elem( "THREAD_QUEUE_SIZE" ) );
    tpool_init( &thread_pool, i_thrd_pool_size, i_thrd_pool_queue, 0 );
}

pool::~pool()
{
    // tpool_destroy ...
}

void
pool::tpool_init( tpool_t *tpoolp, int num_worker_threads, int max_queue_size, int do_not_block_when_full )
{
    int i, rtn;
    tpool_t tpool;

    // allocate a pool data structure
    if (( tpool = (tpool_t) malloc( sizeof( struct tpool ) ) ) == NULL )
    {
     wrap::system_message( POOLERR );
     exit(-1);
    }

    // initialize th fields
    tpool->num_threads    = num_worker_threads;
    tpool->max_queue_size = max_queue_size;
    tpool->do_not_block_when_full = do_not_block_when_full;

    if ( ( tpool->threads = (pthread_t*) malloc( sizeof( pthread_t ) *num_worker_threads ) ) == NULL )
    {
        wrap::system_message( POOLERR );
        exit(-1);
    }

    tpool->cur_queue_size = 0;
    tpool->queue_head     = NULL;
    tpool->queue_tail     = NULL;
    tpool->queue_closed   = 0;
    tpool->shutdown       = 0;

    if ( ( rtn = pthread_mutex_init( &(tpool->queue_lock), NULL ) ) != 0 )
    {
        string s_err( "pthread_mutex_init " );
        s_err.append( strerror( rtn ) );

        wrap::system_message( s_err );

        exit(-1);
    }
    else if ( ( rtn = pthread_cond_init( &(tpool->queue_not_empty), NULL ) ) != 0 )
    {
        string s_err( "pthread_mutex_init " );
        s_err.append( strerror( rtn ) );

        wrap::system_message( s_err );

        pthread_mutex_unlock( &wrap::MUTX->mut_stdout );
        exit(-1);
    }
    else if ( ( rtn = pthread_cond_init( &(tpool->queue_not_full), NULL ) ) != 0 )
    {
        string s_err( "pthread_mutex_init " );
        s_err.append( strerror( rtn ) );

        wrap::system_message( s_err );

        exit(-1);
    }
    else if ( ( rtn = pthread_cond_init( &(tpool->queue_empty), NULL ) ) != 0 )
    {
        string s_err( "pthread_mutex_init " );
        s_err.append( strerror( rtn ) );

        wrap::system_message( s_err );

        exit(-1);
    }
    // create threads
    for ( i = 0; i < num_worker_threads; i++ )
        pthread_create( &(tpool->threads[i]) , NULL, tpool_thread, (void*)tpool );

    *tpoolp = tpool;
}

void*
pool::tpool_thread( void* arg )
{
    tpool_t tpool = (tpool_t) arg;
    tpool_work_t *my_workp;

    while( true )
    {
        pthread_mutex_lock( &(tpool->queue_lock) );

        while ( (tpool->cur_queue_size == 0) && (!tpool->shutdown) )
            pthread_cond_wait( &(tpool->queue_not_empty), &(tpool->queue_lock) );

        if (tpool->shutdown)
        {
            pthread_mutex_unlock( &(tpool->queue_lock) );
            pthread_exit( NULL );
        }

        my_workp = tpool->queue_head;
        tpool->cur_queue_size--;

        if ( tpool->cur_queue_size == 0)
            tpool->queue_head = tpool->queue_tail = NULL;

        else
            tpool->queue_head = my_workp->next;

        if ( ( ! tpool->do_not_block_when_full ) &&
                (   tpool->cur_queue_size == ( tpool->max_queue_size - 1 ) ) )
            pthread_cond_signal( &(tpool->queue_not_full) );

        if ( tpool->cur_queue_size == 0 )
            pthread_cond_signal( &(tpool->queue_empty) );

        pthread_mutex_unlock( &(tpool->queue_lock) );
        (*(my_workp->routine))(my_workp->arg);
        free((void*)my_workp);
    }
}

void pool::run_func( void *v_pointer )
{
    wrap::SOCK->increase_num_threads();

    // recasting the client thread object.
    thrd *t = (thrd*) v_pointer;

    // start parsing the client request and sending response's back.
    t-> run ();

    // close the client socket.
    t->~thrd();

    wrap::SOCK->decrease_num_threads();

    free(v_pointer);
}

int
pool::tpool_add_work( tpool_t tpool, void(*routine)(void*), void* arg ) ///
{
    tpool_work_t *workp;
    pthread_mutex_lock( &(tpool->queue_lock) );

    if( ( tpool->cur_queue_size == tpool->max_queue_size ) &&
            tpool->do_not_block_when_full )
    {
        pthread_mutex_unlock( &(tpool->queue_lock) );
        return -1;
    }

    while( (     tpool->cur_queue_size == tpool->max_queue_size ) &&
            ( ! ( tpool->shutdown       || tpool->queue_closed   ) ) )
        pthread_cond_wait( &(tpool->queue_not_full), &(tpool->queue_lock) );

    if( tpool->shutdown || tpool->queue_closed )
    {
        pthread_mutex_unlock( &tpool->queue_lock );
        return -1;
    }

    // allocate work structure:
    workp = (tpool_work_t*) malloc( sizeof( tpool_work_t ) );

    workp->routine = routine;
    workp->arg     = arg;
    workp->next    = NULL;

    if( tpool->cur_queue_size == 0 )
    {
        tpool->queue_tail = tpool->queue_head = workp;
    }
    else
    {
        (tpool->queue_tail)->next = workp;
        tpool->queue_tail         = workp;
    }

    pthread_cond_signal( &tpool->queue_not_empty );
    tpool->cur_queue_size++;
    pthread_mutex_unlock( &(tpool->queue_lock) );
    return 1;
}

#endif
