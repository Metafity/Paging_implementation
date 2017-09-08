#include <iostream>
#include <cstdlib>
#define MAX_PROCESSES 1000

using namespace std;


class Frame {
    bool avail;
public:
    Frame(){
        avail = true;
    };
    void free() {
        avail = true;
    }
    void occupy() {
        avail = false;
    }
    bool isFree() {
        return avail;
    };
};

class Process {
    int p_id;
    int* frames;
    int numframes;
public:
    Process(int size,int psize, int pid) {
        p_id = pid;
        frames = new int[size / psize];
        numframes = 0;
    }
    ~Process() {
        delete frames;
        frames=NULL;
    }
    void addFrame(int i) {
        frames[numframes++] = i;
    }
    int numFrames() {
        return numframes;
    }
    int getPid() {
        return p_id;
    }
    int getFrame(int pnum) {
        if(pnum >= numframes) throw("Invalid pagenumber\n");
        return (*(frames + pnum));
    }
    int* getFrames() {
        return frames;
    }
};

class Memory {
    Frame* mem;
    int psize;
    int numframe;
    int freeframes;
    Process* pids[MAX_PROCESSES];
    int numProcess;

public:
    Memory(int memsize, int psize) {
        numProcess = 0;
        this-> psize = psize;
        numframe = memsize / psize;
        freeframes = numframe;
        mem = new Frame[numframe];
    }

    int AddProcess(int size) {
        if(freeframes == 0) throw("Insufficient memory\n");

        pids[numProcess]  = new Process(size, psize, numProcess);
        for(int i=0; i < (size / psize); i++) {

            int framenum;

            do framenum = rand() % numframe;
            while(!(mem + framenum)->isFree());

            cout<<"Assigning frame "<<framenum<<endl;

            freeframes--;
            (mem + framenum)->occupy();
            (pids[numProcess])->addFrame(framenum);
        }

        return numProcess++;
    }

    void RemProcess(int pid) {
        for(int i = 0; i < numProcess; i++) {
            if((pids[i])->getPid() == pid) {
                int* frames = (pids[i])->getFrames();
                for(int j = 0; j < (pids[i])->numFrames(); j++) {
                    (mem + *(frames+j) )->free();
                    freeframes++;
                }
                delete (pids[i]);
                for(int k = i; k < numProcess - 1; k++) {
                    pids[i] = pids[i+1];
                }
                numProcess--;
                return;
            }
        }
        throw("Invalid pid\n");
    }
    int GetFrameNum(int pid, int pagenum) {
        for(int i = 0;i < numProcess; i++) {
            if((pids[i])->getPid() == pid) {
                if(pagenum > (pids[i])->numFrames()) throw("Invalid page number");
                return ((pids[i])->getFrame(pagenum));
            }
        }
        throw("Invalid pid\n");
    }
};
int main(int argc,char* argv[])
{
    try {
        if(argc!= 3) throw ("Improper arguments\n");
        int memory= atoi(argv[1]);
        int psize= atoi(argv[2]);
        Memory m(memory, psize);
        cout<<"Memory initialised with size "<<memory<<" and page size "<<psize<<endl;
        char c;
        int temp;
        int mod;
        while(true) {
            cout<<"\n<--------Choose an operation-------->"<<endl;
            cout<<"1. Add a process"<<endl;
            cout<<"2. Remove a process"<<endl;
            cout<<"3. Get frame number for a process"<<endl;
            cout<<"4. Quit"<<endl;
            cin>>c;

            switch(c) {
            case '1':
                cout<<"Enter size of process"<<endl;
                cin>>temp;
                mod = (temp % psize == 0)? 0 : 1;
                temp = temp / psize;
                temp = m.AddProcess((temp + mod) * psize);
                cout<<"Process assigned pid "<<temp<<endl;
                break;
            case '2':
                cout<<"Enter pid of process to be removed"<<endl;
                cin>>temp;
                m.RemProcess(temp);
                break;
            case '3':
                int temp2;
                cout<<"Enter pid and page number"<<endl;
                cin>>temp>>temp2;
                cout<<"Frame number is "<<m.GetFrameNum(temp, temp2)<<endl;
                break;
            default:
                cout<<"Quiting"<<endl;
                return 0;
            }
        }
    }
    catch(const char* err) {
        cout<<err<<endl;
    }
}
