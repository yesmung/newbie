# -*- coding: utf-8 -*-

import numpy as np
import cv2

from tensorflow.python.framework import ops
from tensorflow.python.ops import math_ops
from tensorflow.python.ops import array_ops
from tensorflow.python.framework import dtypes

import tensorflow as tf


def rescale_images(image):
    image = image * 127.5 + 127.5
    return np.array(image, dtype=np.uint8)


def rescale_maps(image):
    image = image * 127.5 + 127.5
    return np.array(image, dtype=np.uint8)


def rescale_maps_for_inference(image):
    image = image * 127.5 + 127.5
    return np.array(image/255., dtype=np.float32)


def per_image_standardization(image):
    image = (tf.cast(image, tf.float32) - 127.5) / 127.5
    return image


def per_image_standardization_map(image):
    image = (tf.cast(image, tf.float32) - 127.5) / 127.5
    return image


def tf_equalize_histogram(image):
    values_range = tf.constant([0., 255.], dtype=tf.float32)
    histogram = tf.histogram_fixed_width(image, values_range, 256)
    cdf = tf.cumsum(histogram)
    cdf_min = cdf[tf.reduce_min(tf.where(tf.greater(cdf, 0)))]

    img_shape = tf.shape(image)
    pix_cnt = img_shape[-3] * img_shape[-2]
    px_map = tf.round(tf.cast(cdf - cdf_min, dtypes.float32) * 255. / tf.cast(pix_cnt - 1, dtypes.float32))
    px_map = tf.cast(px_map, tf.uint8)

    eq_hist = tf.expand_dims(tf.gather_nd(px_map, tf.cast(image, tf.int32)), 2)
    return eq_hist


def compute_input(image):
    # should be RGB order
    image = image.astype('float32')
    mean = np.array([0.485, 0.456, 0.406])
    variance = np.array([0.229, 0.224, 0.225])

    image -= mean * 255
    image /= variance * 255
    return image


def invert_input(x):
    X = X.copy()
    mean = np.array([0.485, 0.456, 0.406])
    variance = np.array([0.229, 0.224, 0.225])

    X *= variance * 255
    X += mean * 255
    return X.clip(0, 255).astype('uint8')


def get_gaussian_heatmap(size=512, distanceRatio=3.34):
    v = np.abs(np.linspace(-size / 2, size / 2, num=size))
    x, y = np.meshgrid(v, v)
    g = np.sqrt(x ** 2 + y ** 2)
    g *= distanceRatio / (size / 2)
    g = np.exp(-(1 / 2) * (g ** 2))
    g *= 255
    return g.clip(0, 255)


def get_gaussian1d_heatmap(size=512, distanceRatio=1.5):
    # 1d gaussian heatmap along x axis
    v = np.abs(np.linspace(-size / 2, size / 2, num=size))
    x, y = np.meshgrid(v, v)
    g = np.sqrt(y ** 2)
    g *= distanceRatio / (size / 2)
    g = np.exp(-(1 / 2) * (g ** 2))
    g *= 255
    return g.clip(0, 255)


def get_asymmetric_gaussian_heatmap(size=512, distanceRatio=3.34, sx=1, sy=1, amp=1):
    v = np.abs(np.linspace(-size / 2, size / 2, num=size))
    x, y = np.meshgrid(v, v)
    g = np.sqrt(sx * x ** 2 + sy * y ** 2)
    g *= distanceRatio / (size / 2)
    g = np.exp(-(1 / 2) * (g ** 2))
    g = g.astype('float32')
    g *= 255
    g *= amp
    return g.clip(0, 255)


def montage_img(im, spsize=(1000,2000), overlap=40):
    imsize = (im.shape[1],im.shape[0])
    numy = int(np.ceil(imsize[1]/spsize[1]))+1
    numx = int(np.ceil(imsize[0]/spsize[0]))+1
    #patchim = np.zeros((numx*numy, spsize[1],spsize[0],3))
    patchim = np.zeros((numx * numy, spsize[1]+overlap*2, spsize[0]+overlap*2, 3))

    largeim = np.zeros((spsize[1]*numx, spsize[0]*numy, 3))
    largeim[0:imsize[1],0:imsize[0],:] = np.array(im)

    ind = 0
    # split
    # plt.subplots(numy-1, numx-1,figsize=(10,10))
    for yidx in range(numx-1):
        for xidx in range(numy-1):
            if yidx == 0 or xidx == 0:
                patchim[ind, overlap:,overlap:,:] = largeim[(yidx * spsize[1]):((yidx + 1) * spsize[1] + overlap),
                                  (xidx * spsize[0]):((xidx + 1) * spsize[0] + overlap), :]
            elif yidx == numx-2 or xidx == numy-2:
                patchim[ind, :-overlap,:-overlap,:] = largeim[(yidx * spsize[1] - overlap):((yidx + 1) * spsize[1] ),
                                  (xidx * spsize[0] - overlap):((xidx + 1) * spsize[0] ), :]
            else:
                patchim[ind,:] = largeim[(yidx*spsize[1]-overlap):((yidx+1)*spsize[1]+overlap),
                                         (xidx*spsize[0]-overlap):((xidx+1)*spsize[0]+overlap),:]
            # plt.subplot(numy-1, numx-1, ind+1)
            # plt.imshow(patchim[ind,:])
            ind = ind + 1
    return patchim, numx, numy, spsize


def merge_montage(patchim, numx, numy, imsize, spsize=(1000, 2000), overlap=20):
    # merge
    ind = 0
    mergeim = np.zeros((spsize[1]*numy, spsize[0]*numx, 3), dtype=np.float32)
    #print(mergeim.shape)

    for yidx in range(numx-1):
        for xidx in range(numy - 1):
            mergeim[yidx*spsize[1]:(yidx+1)*spsize[1],xidx*spsize[0]:(xidx+1)*spsize[0],:] = patchim[ind,overlap:-overlap,overlap:-overlap,:]
            # plt.subplot(numy-1, numx-1, ind+1)
            # plt.imshow(patchim[ind,:])
            ind = ind + 1

    return mergeim[0:imsize[0],0:imsize[1],:]


def compute_word_maps_no_space(heatmap, image_height, image_width, lines, descale=2, shiftx=0, shifty=0, paddx=0, paddy=0):

    textmap = np.zeros((image_height // descale, image_width // descale)).astype('float32')

    src = np.array([[0, 0], [heatmap.shape[1], 0], [heatmap.shape[1], heatmap.shape[0]], [0, heatmap.shape[0]]]).astype(
        'float32')

    for line in lines:
        for lind in range(len(line)):
            lvals = line[lind]
            x1 = lvals[0] - paddx - shiftx
            y1 = lvals[1] - paddy - shifty

            x2 = lvals[2] + paddx - shiftx
            y2 = lvals[3] - paddy - shifty

            x3 = lvals[4] + paddx - shiftx
            y3 = lvals[5] + paddy - shifty

            x4 = lvals[6] - paddx - shiftx
            y4 = lvals[7] + paddy - shifty
            c = lvals[8]

            character_points = np.array([[x1, y1], [x2, y2], [x3, y3], [x4, y4]]).astype(
                'float32') / descale

            MA = cv2.getPerspectiveTransform(
                src=src,
                dst=character_points,
            )

            textmap += cv2.warpPerspective(heatmap, MA, dsize=(textmap.shape[1], textmap.shape[0])).astype(
                'float32')

    emptymap = np.zeros(textmap.shape)
    return np.concatenate([textmap[..., np.newaxis], emptymap[..., np.newaxis], emptymap[..., np.newaxis]],
                            axis=2)


def compute_word_maps(heatmap, image_height, image_width, lines, heatmap_affinity=None, descale=2, make_channel=False):
    # if heatmap_affinity is None:
    #     heatmap_affinity = heatmap

    textmap = np.zeros((image_height // descale, image_width // descale)).astype('float32')
    # linkmap = np.zeros((image_height // descale, image_width // descale)).astype('float32')

    src = np.array([[0, 0], [heatmap.shape[1], 0], [heatmap.shape[1], heatmap.shape[0]], [0, heatmap.shape[0]]]).astype(
        'float32')

    for line in lines:
        previous_link_points = False

        for lind in range(len(line)):
            lvals = line[lind]
            paddx = 3
            paddy = 1
            x1 = lvals[0] - paddx
            y1 = lvals[1] - paddy

            x2 = lvals[2] + paddx
            y2 = lvals[3] - paddy

            x3 = lvals[4] + paddx
            y3 = lvals[5] + paddy

            x4 = lvals[6] - paddx
            y4 = lvals[7] + paddy
            c = lvals[8]

            if previous_link_points is False:
                rx1 = x1
                rx2 = x2
                rx3 = x3
                rx4 = x4
                ry1 = y1
                ry2 = y2
                ry3 = y3
                ry4 = y4

            rx1 = np.min([x1, rx1])
            ry1 = np.min([y1, ry1])

            rx2 = np.max([x2, rx2])
            ry2 = np.min([y2, ry2])

            rx3 = np.max([x3, rx3])
            ry3 = np.max([y3, ry3])

            rx4 = np.min([x4, rx4])
            ry4 = np.max([y4, ry4])

            if c == ' ' and previous_link_points == True:
                previous_link_points = False
                character_points = np.array([[rx1, ry1], [rx2, ry2], [rx3, ry3], [rx4, ry4]]).astype(
                    'float32') / descale

                MA = cv2.getPerspectiveTransform(
                    src=src,
                    dst=character_points,
                )

                textmap += cv2.warpPerspective(heatmap, MA, dsize=(textmap.shape[1], textmap.shape[0])).astype(
                    'float32')

                continue
            previous_link_points = True

    emptymap = np.zeros(textmap.shape)
    return np.concatenate([textmap[..., np.newaxis], emptymap[..., np.newaxis], emptymap[..., np.newaxis]],
                            axis=2)


def compute_maps(heatmap, image_height, image_width, lines, heatmap_affinity=None, descale=2, make_channel=False):
    if heatmap_affinity is None:
        heatmap_affinity = heatmap

    textmap = np.zeros((image_height // descale, image_width // descale)).astype('float32')
    linkmap = np.zeros((image_height // descale, image_width // descale)).astype('float32')

    src = np.array([[0, 0], [heatmap.shape[1], 0], [heatmap.shape[1], heatmap.shape[0]], [0, heatmap.shape[0]]]).astype(
        'float32')

    for line in lines:
        previous_link_points = None

        for lind in range(len(line)):
            lvals = line[lind]
            paddx = 3
            paddy = 1
            x1 = lvals[0] - paddx
            y1 = lvals[1] - paddy

            x2 = lvals[2] + paddx
            y2 = lvals[3] - paddy

            x3 = lvals[4] + paddx
            y3 = lvals[5] + paddy

            x4 = lvals[6] - paddx
            y4 = lvals[7] + paddy
            c = lvals[8]

            if c == ' ':
                previous_link_points = None
                continue

            yc = (y4 + y1 + y3 + y2) / (2 * descale)
            xc = (x1 + x2 + x3 + x4) / (2 * descale)

            current_link_points = np.array([[(xc + (x1 + x2) / descale) / 2,
                                             (yc + (y1 + y2) / descale) / 2],
                                            [(xc + (x3 + x4) / descale) / 2,
                                             (yc + (y3 + y4) / descale) / 2]]) / 2
            character_points = np.array([[x1, y1], [x2, y2], [x3, y3], [x4, y4]]).astype('float32') / descale

            # pylint: disable=unsubscriptable-object
            if previous_link_points is not None:
                link_points = np.array([
                    previous_link_points[0], current_link_points[0], current_link_points[1],
                    previous_link_points[1]
                ])
                ML = cv2.getPerspectiveTransform(
                    src=src,
                    dst=link_points.astype('float32'),
                )
                linkmap += cv2.warpPerspective(heatmap_affinity,
                                               ML,
                                               dsize=(linkmap.shape[1],
                                                      linkmap.shape[0])).astype('float32')
            MA = cv2.getPerspectiveTransform(
                src=src,
                dst=character_points,
            )
            textmap += cv2.warpPerspective(heatmap, MA, dsize=(textmap.shape[1], textmap.shape[0])).astype('float32')

            # pylint: enable=unsubscriptable-object
            previous_link_points = current_link_points

    if make_channel == False:
        return np.concatenate([textmap[..., np.newaxis], linkmap[..., np.newaxis]], axis=2).clip(0, 255)
    else:
        emptymap = np.zeros(textmap.shape)
        return np.concatenate([textmap[..., np.newaxis], linkmap[..., np.newaxis], emptymap[..., np.newaxis]],
                              axis=2)

def map_to_rgb(y):
    return (np.concatenate([y, np.zeros((y.shape[0], y.shape[1], 1))], axis=-1) * 255).astype('uint8')


def drawBoxes(image, boxes, color=(255, 0, 0), thickness=5):
    canvas = image.copy()
    for box in boxes:
        cv2.polylines(img=canvas,
                      pts=box[np.newaxis].astype('int32'),
                      color=color,
                      thickness=thickness,
                      isClosed=True)
    return canvas


def getBoxes(y_pred, detection_threshold=0.7, text_threshold=0.4, link_threshold=0.4, size_threshold=10, dscale=2):
    box_groups = []
    for y_pred_cur in y_pred:
        # Prepare data
        textmap = y_pred_cur[..., 0].copy()
        linkmap = y_pred_cur[..., 1].copy()
        img_h, img_w = textmap.shape

        _, text_score = cv2.threshold(textmap, thresh=text_threshold, maxval=1, type=cv2.THRESH_BINARY)
        _, link_score = cv2.threshold(linkmap, thresh=link_threshold, maxval=1, type=cv2.THRESH_BINARY)

        n_components, labels, stats, _ = cv2.connectedComponentsWithStats(
            np.clip(text_score + link_score, 0, 1).astype('uint8'), connectivity=4)

        boxes = []
        for component_id in range(1, n_components):
            # Filter by size
            size = stats[component_id, cv2.CC_STAT_AREA]

            if size < size_threshold:
                continue

            # If the maximum value within this connected component is less than
            # text threshold, we skip it.
            if np.max(textmap[labels == component_id]) < detection_threshold:
                continue

            # Make segmentation map. It is 255 where we find text, 0 otherwise.
            segmap = np.zeros_like(textmap)
            segmap[labels == component_id] = 255
            segmap[np.logical_and(link_score, text_score)] = 0
            x, y, w, h = [
                stats[component_id, key] for key in
                [cv2.CC_STAT_LEFT, cv2.CC_STAT_TOP, cv2.CC_STAT_WIDTH, cv2.CC_STAT_HEIGHT]
            ]

            # Expand the elements of the segmentation map
            niter = int(np.sqrt(size * min(w, h) / (w * h)) * dscale)
            sx, sy = max(x - niter, 0), max(y - niter, 0)
            ex, ey = min(x + w + niter + 1, img_w), min(y + h + niter + 1, img_h)

            segmap[sy:ey, sx:ex] = cv2.dilate(
                segmap[sy:ey, sx:ex],
                cv2.getStructuringElement(cv2.MORPH_RECT, (1 + niter, 1 + niter)))

            # Make rotated box from contour
            contours, _ = cv2.findContours(segmap.astype('uint8'), mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_SIMPLE)
            contour = contours[0]
            box = cv2.boxPoints(cv2.minAreaRect(contour))

            # Check to see if we have a diamond
            w, h = np.linalg.norm(box[0] - box[1]), np.linalg.norm(box[1] - box[2])
            box_ratio = max(w, h) / (min(w, h) + 1e-5)
            if abs(1 - box_ratio) <= 0.1:
                l, r = contour[:, 0, 0].min(), contour[:, 0, 0].max()
                t, b = contour[:, 0, 1].min(), contour[:, 0, 1].max()
                box = np.array([[l, t], [r, t], [r, b], [l, b]], dtype=np.float32)
            else:
                # Make clock-wise order
                box = np.array(np.roll(box, 4 - box.sum(axis=1).argmin(), 0))
            boxes.append(dscale * box)
        box_groups.append(np.array(boxes))
    return box_groups


def getTextBoxes(y_pred, detection_threshold=0.7, text_threshold=0.4, size_threshold=10, dscale=2):
    box_groups = []
    for y_pred_cur in y_pred:
        # Prepare data
        textmap = y_pred_cur[..., 0].copy()
        linkmap = y_pred_cur[..., 1].copy()
        img_h, img_w = textmap.shape

        _, text_score = cv2.threshold(textmap, thresh=text_threshold, maxval=1, type=cv2.THRESH_BINARY)

        n_components, labels, stats, _ = cv2.connectedComponentsWithStats(
            np.clip(text_score, 0, 1).astype('uint8'), connectivity=4)

        boxes = []
        for component_id in range(1, n_components):
            # Filter by size
            size = stats[component_id, cv2.CC_STAT_AREA]

            if size < size_threshold:
                continue

            # If the maximum value within this connected component is less than
            # text threshold, we skip it.
            if np.max(textmap[labels == component_id]) < detection_threshold:
                continue

            # Make segmentation map. It is 255 where we find text, 0 otherwise.
            segmap = np.zeros_like(textmap)
            segmap[labels == component_id] = 255
            # segmap[np.logical_and(text_score, text_score)] = 0
            x, y, w, h = [
                stats[component_id, key] for key in
                [cv2.CC_STAT_LEFT, cv2.CC_STAT_TOP, cv2.CC_STAT_WIDTH, cv2.CC_STAT_HEIGHT]
            ]

            # Expand the elements of the segmentation map
            niter = int(np.sqrt(size * min(w, h) / (w * h)) * dscale)
            sx, sy = max(x - niter, 0), max(y - niter, 0)
            ex, ey = min(x + w + niter + 1, img_w), min(y + h + niter + 1, img_h)

            segmap[sy:ey, sx:ex] = cv2.dilate(
                segmap[sy:ey, sx:ex], cv2.getStructuringElement(cv2.MORPH_RECT, (1 + niter, 1 + niter)))

            # Make rotated box from contour
            contours, _ = cv2.findContours(segmap.astype('uint8'), mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_SIMPLE)
            contour = contours[0]
            box = cv2.boxPoints(cv2.minAreaRect(contour))

            # Check to see if we have a diamond
            w, h = np.linalg.norm(box[0] - box[1]), np.linalg.norm(box[1] - box[2])
            box_ratio = max(w, h) / (min(w, h) + 1e-5)
            if abs(1 - box_ratio) <= 0.1:
                l, r = contour[:, 0, 0].min(), contour[:, 0, 0].max()
                t, b = contour[:, 0, 1].min(), contour[:, 0, 1].max()
                box = np.array([[l, t], [r, t], [r, b], [l, b]], dtype=np.float32)
            else:
                # Make clock-wise order
                box = np.array(np.roll(box, 4 - box.sum(axis=1).argmin(), 0))
            boxes.append(dscale * box)
        box_groups.append(np.array(boxes))
    return box_groups
