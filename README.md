# img-dup
This is a quick and dirty QT program to identify duplicate and near-duplicate images in a collection of photos.

## Tech and Algorithms
To achieve a perceptual hash, I started using pHash, but it was slow and unwieldy.  Instead, I'm using the opencv img_hash module.  The hash algorithm is a template parameter, so swapping the actual hash should be simple.  To avoid the `O(n^2)` comparisons of hashes, I use a metric tree (Vantage Point tree), adapting an implementation by Steven Hanov.  This allows for the closest image matches to be retrieved in around `O(log(n))` time per each base image compared against.  This brings the total complexity to `O(n log(n))`, but the bulk of the work is in the `O(n)` preprocessing stage.

## Usage
After starting the program, you may begin preprocessing from the system menu, and once preprocessing is done, the main view will be prevented.  Within this view, an image is shown on the left, and possible duplicates are shown on the right.  To select a duplicate, use arrow keys, and to delete the duplicate, press `d` on the keyboard.  To move to the next image in the collection, press `n`.