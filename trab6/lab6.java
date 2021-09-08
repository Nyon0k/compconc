class S{
	private int r;

	public S(){
		this.r = 0;
	}

	public synchronized void inc(){
		this.r++;
	}

	public synchronized int get(){
		return this.r;
	}
}

class T extends Thread{
	private int id;
	public int[] vet;
	public int nthreads;
	public int divVet;
	public int divVetResto;
	S s;

	public T(int tid, int[] vet, int nthreads, S s){
		this.id = tid;
		this.vet = vet;
		this.nthreads = nthreads;
		this.s = s;
	}

	public void run(){
		for(int i=this.id; i<this.vet.length; i+=nthreads){
			if(this.vet[i]%2 == 0){
				this.s.inc();
			}
		}
	}
}

class lab6{
	public static void main (String[] args){
		if(args.length < 2){
			System.out.println("java lab6 <numero de elementos> <numero de threads>");
			System.exit(1);
		}

		int tam = Integer.parseInt(args[0]);
		int nthreads = Integer.parseInt(args[1]) > tam ? tam : Integer.parseInt(args[1]);
		T[] threads = new T[nthreads];

		S s = new S();

		// System.out.println("Vetor:");
		int[] vet = new int[tam];
		for(int i=0; i<tam; i++){
			vet[i] = i+1;
			// System.out.println(vet[i]);
		}

		int divVetSum = 0;
		for(int i=0; i<threads.length; i++){
			threads[i] = new T(i, vet, nthreads, s);
		}

		for(int i=0; i<threads.length; i++){
			threads[i].start();
		}

		for(int i=0; i<threads.length; i++){
			try{
				threads[i].join();
			}catch(InterruptedException e){
			return;
			}
		}

		System.out.println("\nQuantidade de numeros pares: " + s.get() + "\n");
	}
}
