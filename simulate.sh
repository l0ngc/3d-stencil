export LD_LIBRARY_PATH=/chalmers/sw/sup64/ansys-2021r1/libpng/lib:${LD_LIBRARY_PATH}
export GEM5_PATH=/chalmers/users/longc/Documents/eda284/lab1/gem5

name="$1"

dir="$GEM5_PATH/m5out/3d_stencil/$name"

stencil_addr="/chalmers/users/longc/Documents/3d-stencil/stencil-3d"

l1d_size="8kB"
l2_size="64kB"

if [ -d "$dir" ]; then
    rm -rf "$dir"
fi

mkdir "$dir"

$GEM5_PATH/build/ARM/gem5.opt -d $GEM5_PATH/m5out/3d_stencil/$name \
    $GEM5_PATH/configs/example/se.py \
    --interp-dir /usr/aarch64-linux-gnu \
    --redirects /lib=/usr/aarch64-linux-gnu/lib \
    --param 'system.cpu[:].isa[:].sve_vl_se = 8' \
    --cpu-type=ex5_big --caches --l2cache --l1d_size=$l1d_size --l2_size=$l2_size -c $stencil_addr
