namespace TreeHash{

	const static int maxn = 1e5 + 50;
	const static int mod = 1e9 + 7;
	const static int p = 233;

	inline void upd( int & x , int v ){
		x += v;
		if( x >= mod )
			x -= mod;
	}

	inline int mul( int x , int y ){
	    return 1LL * x * y % mod;
	}

	int dp[maxn] , f[maxn] , sz[maxn] , power[maxn] , prefix[maxn] , suffix[maxn] , n;
	pair < int , int > seq[maxn];
	vector < int > Edge[maxn];

	void PreDeal(){
		power[0] = 1;
    	for(int i = 1 ; i < maxn ; ++ i)
        	power[i] = mul( power[i - 1] , p );
	}

	void DFS( int u , int fa ){
	    dp[u] = 0;
	    sz[u] = 1;
	    for(auto v : Edge[u]){
	        if( v == fa )
	            continue;
	        DFS( v , u );
	        sz[u] += sz[v];
	    }
	    int len = 0;
	    for(auto v : Edge[u]){
	        if( v == fa )
	            continue;
	        seq[ ++ len] = make_pair( dp[v] , sz[v] );
	    }
	    sort( seq + 1 , seq + len + 1 );
	    dp[u] = sz[u];
	    for(int i = 1 ; i <= len ; ++ i){
	        dp[u] = mul( dp[u] , power[seq[i].second] );
	        upd( dp[u] , seq[i].first );
	    }
	}

	void Cal( int u , int fa ){
	    int len = 0;
	    for(auto v : Edge[u]){
	        if( v == fa )
	            seq[ ++ len ] = make_pair( f[u] , n - sz[u] );
	        else
	            seq[ ++ len ] = make_pair( dp[v] , sz[v] );
	    }
	    sort( seq + 1 , seq + len + 1 );
	    prefix[0] = 0;
	    suffix[len + 1] = 0;
	    for(int i = 1 ; i <= len ; ++ i){
	        prefix[i] = mul( prefix[i - 1] , power[seq[i].second] );
	        upd( prefix[i] , seq[i].first );
	    }
	    for(int i = len ; i >= 1 ; -- i)
	        suffix[i] = suffix[i + 1] + seq[i].second;
	    int hashu = prefix[len];
	    f[u] = hashu;
	    for(auto v : Edge[u]){
	        if( v == fa )
	            continue;
	        else{
	            int id = lower_bound( seq + 1 , seq + len + 1 , make_pair( dp[v] , sz[v] ) ) - seq;
	            int ret = mul( power[suffix[id + 1]] , prefix[id] );
	            ret = mod - ret;
	            int newhash = hashu;
	            upd( newhash , ret );
	            upd( newhash , mul( prefix[id - 1] , power[suffix[id + 1]] ) );
	            f[v] = newhash;
	            upd( f[v] , mul( n - sz[v] , power[ n - 1 - sz[v] ] ) );
	        }
	    }
	    upd( f[u] , mul( n , power[n - 1] ) );
	    for(auto v : Edge[u]){
	        if( v == fa )
	            continue;
	        Cal( v , u );
	    }
	}
};