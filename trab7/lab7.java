
class LE {
    private int leit, escr;  
  
    LE() { 
        this.leit = 0;
        this.escr = 0;
    } 
  
    public synchronized void EntraLeitor (int id) {
        try { 
            while (this.escr > 0) {
                System.out.println ("le.leitorBloqueado("+id+")");
                wait();
            }
            this.leit++;
            System.out.println ("le.leitorLendo("+id+")");
        }catch(InterruptedException e){ }
    }
  
    public synchronized void SaiLeitor (int id) {
        this.leit--;
        if (this.leit == 0) 
            this.notify();
        System.out.println ("le.leitorSaindo("+id+")");
    }
  
    public synchronized void EntraEscritor (int id) {
        try { 
        while ((this.leit > 0) || (this.escr > 0)) {
            System.out.println ("le.escritorBloqueado("+id+")");
            wait();
        }
        this.escr++;
        System.out.println ("le.escritorEscrevendo("+id+")");
        } catch (InterruptedException e) { }
    }
    
    public synchronized void SaiEscritor (int id) {
        this.escr--;
        notifyAll();
        System.out.println ("le.escritorSaindo("+id+")");
    }
}

class Leitor extends Thread {
    int id;
    LE monitor;

    Leitor(int id, LE monitor){
        this.id = id;
        this.monitor = monitor;
    }

    public void run () {
        monitor.EntraLeitor(this.id);
        int central = lab7.central;
        if(central<=1){
            System.out.println("\nL" + this.id + "; " + central + " não é primo.\n");
        }else{
            int flag = 0;
            for(int i=2; i<=central/2; i++){
                if(central%i==0){
                    flag += 1;
                }
            }
            if(flag==0 || flag==1){
                System.out.println("\nL" + this.id + "; " + central + " é primo.\n");
            }
            if(flag==2){
                System.out.println("\nL" + this.id + "; " + central + " não é primo.\n");
            }
        }
        monitor.SaiLeitor(this.id);
    }
}

class Escritor extends Thread {
    int id;
    LE monitor;

    Escritor (int id, LE monitor) {
        this.id = id;
        this.monitor = monitor;
    }

    public void run() {
        monitor.EntraEscritor(this.id);
        lab7.central = this.id;
        System.out.println("\nE" + this.id + "; central = " + lab7.central + "\n");
        monitor.SaiEscritor(this.id);
    }
}

class LeitorEscritor extends Thread {
    int id;
    LE monitor;

    LeitorEscritor(int id, LE monitor){
        this.id = id;
        this.monitor = monitor;
    }

    public void run(){
        monitor.EntraLeitor(this.id);
        int central = lab7.central;
        if(central<=1){
            System.out.println("\nLE L" + this.id + "; " + central + " não é primo.\n");
        }else{
            int flag = 0;
            for(int i=2; i<=central/2; i++){
                if(central%i==0){
                    flag += 1;
                }
            }
            if(flag==0 || flag==1){
                System.out.println("\nLE L" + this.id + "; " + central + " é primo.\n");
            }
            if(flag==2){
                System.out.println("\nLE L" + this.id + "; " + central + " não é primo.\n");
            }
        }
        monitor.SaiLeitor(this.id);
        monitor.EntraEscritor(this.id);
        lab7.central = lab7.central * 2;
        System.out.println("\nLE E" + this.id + "; central = " + lab7.central + "\n");
        monitor.SaiEscritor(this.id);
    }
}

class lab7 {
    static int nLE;
    static int nL;
    static int nE;
    static int central = 0;

    public static void main (String[] args) {
        LE monitor = new LE();

        if(args.length < 3){
            System.out.println("java lab6 <numero de LEs> <numero de Ls> <numero de Es>");    
            System.exit(1);
        }
        nLE = Integer.parseInt(args[0]);
        nL = Integer.parseInt(args[1]);
        nE = Integer.parseInt(args[2]);
        // System.out.println(nLE + " " + nL + " " + nE);
        if((nLE>=1) && (nL>=1) && (nE>=1)){}else{
            System.out.println("Informe valores >= 1!");
            System.exit(1);
        }

        int qThreads = nLE + nL + nE;
        Thread t[] = new Thread[qThreads];

        System.out.println ("import verificaLE");
        System.out.println ("le = verificaLE.LE()");

        int initializer = 0;
        for(int i=0; i<nLE; i++){
			t[i] = new LeitorEscritor(i, monitor);
            initializer = nLE;
            // System.out.println ("nLE:"+nLE+" i:"+i);
		}
        // System.out.println ("1 initializer: " + initializer);
        for(int i=initializer; i<nL+initializer; i++){
			t[i] = new Leitor(i, monitor);
            // System.out.println ("nL:"+nL+" i:"+i);
		}
        initializer = nL+initializer;
        // System.out.println ("2 initializer: " + initializer);
        for(int i=initializer; i<nE+initializer; i++){
            if(!(initializer>=qThreads)){
                t[i] = new Escritor(i, monitor);
                // System.out.println ("nE:"+nE+" i:"+i);
            }
		}
        initializer = nE+initializer;
        // System.out.println ("3 initializer: " + initializer);
        
        for (int i=0; i<t.length; i++) {
            t[i].start();
        }

        for(int i=0; i<t.length; i++){
			try{
			    t[i].join();
			}catch(InterruptedException e){
			    return;
			}
		}
    }
}