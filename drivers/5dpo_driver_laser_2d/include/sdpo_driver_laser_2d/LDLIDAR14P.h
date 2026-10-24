//
// Created by afonsotmm on 10/24/26.
//

#ifndef INC_5DPO_RAFT_LDLIDAR14P_H
#define INC_5DPO_RAFT_LDLIDAR14P_H

class LDLIDAR14P : public SdpoDriverLaser2D {
private:

public:
    LDLIDAR14P();
    ~LDLIDAR14P() override;

    bool initialize(const bool dbg = false) override;
    void start();
    void stop();
    void restart();

};


#endif //INC_5DPO_RAFT_LDLIDAR14P_H
