/*************************************************/
/*  image_loader_webp.cpp                        */
/*************************************************/
/*            This file is part of:              */
/*                GODOT ENGINE                   */
/*************************************************/
/*       Source code within this file is:        */
/*  (c) 2007-2010 Juan Linietsky, Ariel Manzur   */
/*             All Rights Reserved.              */
/*************************************************/

#include "image_loader_webp.h"

#include "print_string.h"
#include "os/os.h"
#include "drivers/webp/decode.h"
#include "drivers/webp/encode.h"
#include "io/marshalls.h"
#include <stdlib.h>

static DVector<uint8_t> _webp_lossy_pack(const Image& p_image,float p_quality) {

	ERR_FAIL_COND_V(p_image.empty(),DVector<uint8_t>());

	Image img=p_image;
	if (img.detect_alpha())
		img.convert(Image::FORMAT_RGBA);
	else
		img.convert(Image::FORMAT_RGB);

	Size2 s(img.get_width(),img.get_height());
	DVector<uint8_t> data = img.get_data();
	DVector<uint8_t>::Read r = data.read();

	uint8_t *dst_buff=NULL;
	size_t dst_size=0;
	if (img.get_format()==Image::FORMAT_RGB) {

		dst_size = WebPEncodeRGB(r.ptr(),s.width,s.height,3*s.width,CLAMP(p_quality*100.0,0,100.0),&dst_buff);
	} else {
		dst_size = WebPEncodeRGBA(r.ptr(),s.width,s.height,4*s.width,CLAMP(p_quality*100.0,0,100.0),&dst_buff);
	}

	ERR_FAIL_COND_V(dst_size==0,DVector<uint8_t>());
	DVector<uint8_t> dst;
	dst.resize(4+dst_size);
	DVector<uint8_t>::Write w = dst.write();
	w[0]='W';
	w[1]='E';
	w[2]='B';
	w[3]='P';
	copymem(&w[4],dst_buff,dst_size);
	free(dst_buff);
	w=DVector<uint8_t>::Write();
	return dst;
}

static Image _webp_lossy_unpack(const DVector<uint8_t>& p_buffer) {

	int size = p_buffer.size()-4;
	ERR_FAIL_COND_V(size<=0,Image());
	DVector<uint8_t>::Read r = p_buffer.read();

	ERR_FAIL_COND_V(r[0]!='W' || r[1]!='E' || r[2]!='B' || r[3]!='P',Image());
	WebPBitstreamFeatures features;
	if (WebPGetFeatures(&r[4],size,&features)!=VP8_STATUS_OK) {
		ERR_EXPLAIN("Error unpacking WEBP image:");
		ERR_FAIL_V(Image());
	}

	//print_line("width: "+itos(features.width));
	//print_line("height: "+itos(features.height));
	//print_line("alpha: "+itos(features.has_alpha));

	DVector<uint8_t> dst_image;
	int datasize = features.width*features.height*(features.has_alpha?4:3);
	dst_image.resize(datasize);

	DVector<uint8_t>::Write dst_w = dst_image.write();

	bool errdec=false;
	if (features.has_alpha)	 {
		errdec = WebPDecodeRGBAInto(&r[4],size,dst_w.ptr(),datasize,4*features.width)==NULL;
	} else {
		errdec = WebPDecodeRGBInto(&r[4],size,dst_w.ptr(),datasize,3*features.width)==NULL;

	}

	//ERR_EXPLAIN("Error decoding webp! - "+p_file);
	ERR_FAIL_COND_V(errdec,Image());

	dst_w = DVector<uint8_t>::Write();

	return Image(features.width,features.height,0,features.has_alpha?Image::FORMAT_RGBA:Image::FORMAT_RGB,dst_image);

}


Error ImageLoaderWEBP::load_image(Image *p_image,FileAccess *f) {


	uint32_t size = f->get_len();
	DVector<uint8_t> src_image;
	src_image.resize(size);

	WebPBitstreamFeatures features;

	DVector<uint8_t>::Write src_w = src_image.write();
	f->get_buffer(src_w.ptr(),size);
	ERR_FAIL_COND_V(f->eof_reached(), ERR_FILE_EOF);

	if (WebPGetFeatures(src_w.ptr(),size,&features)!=VP8_STATUS_OK) {
		f->close();
		//ERR_EXPLAIN("Error decoding WEBP image: "+p_file);
		ERR_FAIL_V(ERR_FILE_CORRUPT);
	}

	print_line("width: "+itos(features.width));
	print_line("height: "+itos(features.height));
	print_line("alpha: "+itos(features.has_alpha));

	src_w = DVector<uint8_t>::Write();

	DVector<uint8_t> dst_image;
	int datasize = features.width*features.height*(features.has_alpha?4:3);
	dst_image.resize(datasize);

	DVector<uint8_t>::Read src_r = src_image.read();
	DVector<uint8_t>::Write dst_w = dst_image.write();


	bool errdec=false;
	if (features.has_alpha)	 {
		errdec = WebPDecodeRGBAInto(src_r.ptr(),size,dst_w.ptr(),datasize,4*features.width)==NULL;
	} else {
		errdec = WebPDecodeRGBInto(src_r.ptr(),size,dst_w.ptr(),datasize,3*features.width)==NULL;

	}

	//ERR_EXPLAIN("Error decoding webp! - "+p_file);
	ERR_FAIL_COND_V(errdec,ERR_FILE_CORRUPT);

	src_r = DVector<uint8_t>::Read();
	dst_w = DVector<uint8_t>::Write();

	*p_image = Image(features.width,features.height,0,features.has_alpha?Image::FORMAT_RGBA:Image::FORMAT_RGB,dst_image);


	return OK;

}

void ImageLoaderWEBP::get_recognized_extensions(List<String> *p_extensions) const {
	
	p_extensions->push_back("webp");
}


ImageLoaderWEBP::ImageLoaderWEBP() {

	Image::lossy_packer=_webp_lossy_pack;
	Image::lossy_unpacker=_webp_lossy_unpack;
}


