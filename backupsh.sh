# run with sve
$GEM5_PATH/build/ARM/gem5.opt -d $GEM5_PATH/m5out/3d_stencil/$name \
    $GEM5_PATH/configs/example/se.py --interp-dir /usr/aarch64-linux-gnu --redirects /lib=/usr/aarch64-linux-gnu/lib \
    --param 'system.cpu[:].isa[:].sve_vl_se = 8' \
    --cpu-type=ex5_big --caches -c /chalmers/users/longc/Documents/3d-stencil/stencil-3d
