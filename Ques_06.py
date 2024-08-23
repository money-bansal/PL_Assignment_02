import cv2
import numpy as np
import matplotlib.pyplot as plt

image_path = 'C:\\Users\\dell\\Downloads\\lights.jpg'
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE) 

image_array = np.array(image)

mean_intensity = np.mean(image_array)
std_dev_intensity = np.std(image_array)
min_intensity = np.min(image_array)
max_intensity = np.max(image_array)

print(f"Mean pixel intensity: {mean_intensity}")
print(f"Standard deviation of pixel intensity: {std_dev_intensity}")
print(f"Minimum pixel intensity: {min_intensity}")
print(f"Maximum pixel intensity: {max_intensity}")

plt.figure(figsize=(10, 4))
plt.hist(image_array.ravel(), bins=256, range=(0, 256), color='black', alpha=0.75)
plt.title('Histogram of Pixel Intensities')
plt.xlabel('Pixel Intensity')
plt.ylabel('Frequency')
plt.grid(True)
plt.show()

height, width = image_array.shape
new_size = (width // 2, height // 2)
resized_image = cv2.resize(image_array, new_size, interpolation=cv2.INTER_AREA)

rotated_image = cv2.rotate(resized_image, cv2.ROTATE_90_CLOCKWISE)

plt.figure(figsize=(15, 5))
plt.subplot(1, 3, 1)
plt.imshow(image_array, cmap='gray')
plt.title('Original Image')
plt.axis('off')

plt.subplot(1, 3, 2)
plt.imshow(resized_image, cmap='gray')
plt.title('Resized Image')
plt.axis('off')

plt.subplot(1, 3, 3)
plt.imshow(rotated_image, cmap='gray')
plt.title('Rotated Image')
plt.axis('off')

plt.show()

U, S, VT = np.linalg.svd(image_array, full_matrices=False)

def reconstruct_image(U, S, VT, k):
    S_k = np.diag(S[:k])
    U_k = U[:, :k]
    VT_k = VT[:k, :]
    return np.dot(U_k, np.dot(S_k, VT_k))

k_values = [10, 20, 30]
compressed_images = []

for k in k_values:
    compressed_image = reconstruct_image(U, S, VT, k)
    compressed_images.append(compressed_image)

plt.figure(figsize=(15, 5))
plt.subplot(1, len(k_values) + 1, 1)
plt.imshow(image_array, cmap='gray')
plt.title('Original Image')
plt.axis('off')

for i, k in enumerate(k_values):
    plt.subplot(1, len(k_values) + 1, i + 2)
    plt.imshow(compressed_images[i], cmap='gray')
    plt.title(f'Compressed Image (k={k})')
    plt.axis('off')

plt.show()
