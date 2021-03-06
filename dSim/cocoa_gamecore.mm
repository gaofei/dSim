//
//  cocoa_gamecore.mm
//  dSim
//
//  Created by Hao Xu on 15/3/18.
//  Copyright (c) 2015年 xuhao. All rights reserved.
//

#include "cocoa_gamecore.h"
#include "base_copter.h"
#include "base_gameCore.h"
#include "helicopter_sim.h"
#include "mavlink_unreal.h"

void stl_copter::run()
{
    calc();
}

std::thread *  th;
void cocoa_gameCore::addDemo()
{
    
    NSString * pat = [[NSBundle mainBundle] pathForResource:@"easy" ofType:@"stl" inDirectory:@"models" ];
    const char * pa = [pat UTF8String];
    
    printf("pa %s\n",pa);
    
//    stl_copter * demo = new stl_copter(pa,this);
    helicopter * hc = new helicopter(this);
    stlmodel * stl = new stlmodel(pa,hc);
    static mavlink_unreal * mav_un =
    new mavlink_unreal("100.65.1.5",7777,(xmodel * )hc
                       );
    
    
    [gra addObj:stl];
    phys_list.push_back((xmodel *)hc);
    
    th = new std::thread([&]{
        
        while (true) {
            
            float dtime = 0.06;
            long time = getCurrentTime();
            
            sim(dtime);
            mav_un -> send();
            
            long last = getCurrentTime() - time;
            
            if(last < dtime * 1000000)
                usleep( int(dtime * 1000000  - last));
            
        }
    }
                         );
    
}
void cocoa_gameCore::Loop()
{
//    sim(0.03f);
    [gra setNeedsDisplay:true];
}

//void cocoa_gameCore::pre_sim()
//{
//    for(auto a : phys_list){
//        a->run(deltatime);
//    }
//}

