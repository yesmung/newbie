# CUDA_VISIBLE_DEVICES=-1 python train.py
from cyclegan import CycleGAN
import multiprocessing

def worker(id):
    dataset_name = 'style_real_%05d'%id
    gan = CycleGAN(dataset_name)
    batchlength = 100
    gan.train(epochs=10, batch_size=1, sample_interval=batchlength-1, n_batches=batchlength)
    return

if __name__ == '__main__':
    """
    p = multiprocessing.Pool(processes=12)
    p.map(worker, list(range(360)))
    p.close()
    p.join()
    """
    for id in range(360):
        worker(id)