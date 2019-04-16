// https://github.com/PaulStoffregen/ILI9488_t3
// http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9488-(320x240-TFT-color-display)-library

/***************************************************
  This is our library for the Adafruit  ILI9488 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// <SoftEgg>

//Additional graphics routines by Tim Trzepacz, SoftEgg LLC added December 2015
//(And then accidentally deleted and rewritten March 2016. Oops!)
//Gradient support
//----------------
//		fillRectVGradient	- fills area with vertical gradient
//		fillRectHGradient	- fills area with horizontal gradient
//		fillScreenVGradient - fills screen with vertical gradient
// 	fillScreenHGradient - fills screen with horizontal gradient

//Additional Color Support
//------------------------
//		color565toRGB		- converts 565 format 16 bit color to RGB
//		color565toRGB14		- converts 16 bit 565 format color to 14 bit RGB (2 bits clear for math and sign)
//		RGB14tocolor565		- converts 14 bit RGB back to 16 bit 565 format color

//Low Memory Bitmap Support
//-------------------------
// writeRect8BPP - 	write 8 bit per pixel paletted bitmap
// writeRect4BPP - 	write 4 bit per pixel paletted bitmap
// writeRect2BPP - 	write 2 bit per pixel paletted bitmap
// writeRect1BPP - 	write 1 bit per pixel paletted bitmap

//String Pixel Length support
//---------------------------
//		strPixelLen			- gets pixel length of given ASCII string

// <\SoftEgg>

#ifndef _ILI9488_t3H_
#define _ILI9488_t3H_

#ifdef __cplusplus
#include "Arduino.h"
#include <DMAChannel.h>
#endif

#if defined(__MKL26Z64__)
#error "Sorry, ILI9488_t3 does not work with Teensy LC.  Use Adafruit_ILI9488."
#elif defined(__AVR__)
#error "Sorry, ILI9488_t3 does not work with Teensy 2.0 or Teensy++ 2.0.  Use Adafruit_ILI9488."
#endif

#define  ILI9488_USE_DMAMEM

// Allow us to enable or disable capabilities, particully Frame Buffer and Clipping for speed and size
#ifndef DISABLE_ILI9488_FRAMEBUFFER
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
#define ENABLE_ILI9488_FRAMEBUFFER
//#define SCREEN_DMA_NUM_SETTINGS (((uint32_t)((2 * ILI9488_TFTHEIGHT * ILI9488_TFTWIDTH) / 65536UL))+1)
#define SCREEN_DMA_NUM_SETTINGS 3 // see if making it a constant value makes difference...
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
#define ENABLE_ILI9488_FRAMEBUFFER
//#define SCREEN_DMA_NUM_SETTINGS (((uint32_t)((2 * ILI9488_TFTHEIGHT * ILI9488_TFTWIDTH) / 65536UL))+1)
#define SCREEN_DMA_NUM_SETTINGS 4 // see if making it a constant value makes difference...
#endif
#endif

#define ILI9488_TFTWIDTH  320
#define ILI9488_TFTHEIGHT 480

#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN   0x10
#define ILI9488_SLPOUT  0x11
#define ILI9488_PTLON   0x12
#define ILI9488_NORON   0x13

#define ILI9488_RDMODE  0x0A
#define ILI9488_RDMADCTL  0x0B
#define ILI9488_RDPIXFMT  0x0C
#define ILI9488_RDIMGFMT  0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF 0x28
#define ILI9488_DISPON  0x29

#define ILI9488_CASET   0x2A
#define ILI9488_PASET   0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_RAMRD   0x2E

#define ILI9488_PTLAR    0x30
#define ILI9488_MADCTL   0x36
#define ILI9488_VSCRSADD 0x37
#define ILI9488_PIXFMT   0x3A

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1  0xC0
#define ILI9488_PWCTR2  0xC1
#define ILI9488_PWCTR3  0xC2
#define ILI9488_PWCTR4  0xC3
#define ILI9488_PWCTR5  0xC4
#define ILI9488_VMCTR1  0xC5
#define ILI9488_VMCTR2  0xC7

#define ILI9488_RDID1   0xDA
#define ILI9488_RDID2   0xDB
#define ILI9488_RDID3   0xDC
#define ILI9488_RDID4   0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1
/*
#define ILI9488_PWCTR6  0xFC

*/

// Color definitions
#define ILI9488_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9488_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9488_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9488_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9488_RED         0xF800      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9488_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9488_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0xF81F

#define CL(_r,_g,_b) ((((_r)&0xF8)<<8)|(((_g)&0xFC)<<3)|((_b)>>3))

#define sint16_t int16_t


// Documentation on the ILI9488_t3 font data format:
// https://forum.pjrc.com/threads/54316-ILI9488_t-font-structure-format

typedef struct {
	const unsigned char *index;
	const unsigned char *unicode;
	const unsigned char *data;
	unsigned char version;
	unsigned char reserved;
	unsigned char index1_first;
	unsigned char index1_last;
	unsigned char index2_first;
	unsigned char index2_last;
	unsigned char bits_index;
	unsigned char bits_width;
	unsigned char bits_height;
	unsigned char bits_xoffset;
	unsigned char bits_yoffset;
	unsigned char bits_delta;
	unsigned char line_space;
	unsigned char cap_height;
} ILI9488_t3_font_t;

#define ILI9488_DMA_INIT	0x01 	// We have init the Dma settings
#define ILI9488_DMA_CONT	0x02 	// continuous mode
#define ILI9488_DMA_ACTIVE  0x80    // Is currently active

//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
//#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
//#define C_BASELINE 10 // Centre character baseline
//#define R_BASELINE 11 // Right character baseline

#ifdef __cplusplus
// At all other speeds, ILI9241_KINETISK__pspi->beginTransaction() will use the fastest available clock
#include <SPI.h>
#include <SPI.h>
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
#if F_BUS >= 64000000
#define ILI9488_SPICLOCK 64000000
#define ILI9488_SPICLOCK_READ 4000000
#else
#define ILI9488_SPICLOCK 30000000
#define ILI9488_SPICLOCK_READ 2000000
#endif
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
#define ILI9488_SPICLOCK 30000000
//#define ILI9488_SPICLOCK 72000000
#define ILI9488_SPICLOCK_READ 4000000
#else
#define ILI9488_SPICLOCK 30000000
#define ILI9488_SPICLOCK_READ 2000000
#endif

class ILI9488_t3 : public Print
{
  public:
	ILI9488_t3(SPIClass *SPIWire, uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12);
	void begin(void);
  	void sleep(bool enable);		
	void pushColor(uint16_t color);
	void fillScreen(uint16_t color);
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
			
	void fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
	void fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1, uint16_t color2);
	void fillScreenVGradient(uint16_t color1, uint16_t color2);
	void fillScreenHGradient(uint16_t color1, uint16_t color2);

	void setRotation(uint8_t r);
	void setScroll(uint16_t offset);
	void invertDisplay(boolean i);
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	// Pass 8-bit (each) R,G,B, get back 16-bit packed color
	static uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}

	//color565toRGB		- converts 565 format 16 bit color to RGB
	static void color565toRGB(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b) {
		r = (color>>8)&0x00F8;
		g = (color>>3)&0x00FC;
		b = (color<<3)&0x00F8;
	}
	
	//color565toRGB14		- converts 16 bit 565 format color to 14 bit RGB (2 bits clear for math and sign)
	//returns 00rrrrr000000000,00gggggg00000000,00bbbbb000000000
	//thus not overloading sign, and allowing up to double for additions for fixed point delta
	static void color565toRGB14(uint16_t color, int16_t &r, int16_t &g, int16_t &b) {
		r = (color>>2)&0x3E00;
		g = (color<<3)&0x3F00;
		b = (color<<9)&0x3E00;
	}
	
	//RGB14tocolor565		- converts 14 bit RGB back to 16 bit 565 format color
	static uint16_t RGB14tocolor565(int16_t r, int16_t g, int16_t b)
	{
		return (((r & 0x3E00) << 2) | ((g & 0x3F00) >>3) | ((b & 0x3E00) >> 9));
	}
	
	//uint8_t readdata(void);
	uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

	// Added functions to read pixel data...
	uint16_t readPixel(int16_t x, int16_t y);
	void readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pcolors);
	void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors);

	// writeRect8BPP - 	write 8 bit per pixel paletted bitmap
	//					bitmap data in array at pixels, one byte per pixel
	//					color palette data in array at palette
	void writeRect8BPP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pixels, const uint16_t * palette );

	// writeRect4BPP - 	write 4 bit per pixel paletted bitmap
	//					bitmap data in array at pixels, 4 bits per pixel
	//					color palette data in array at palette
	//					width must be at least 2 pixels
	void writeRect4BPP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pixels, const uint16_t * palette );
	
	// writeRect2BPP - 	write 2 bit per pixel paletted bitmap
	//					bitmap data in array at pixels, 4 bits per pixel
	//					color palette data in array at palette
	//					width must be at least 4 pixels
	void writeRect2BPP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pixels, const uint16_t * palette );
	
	// writeRect1BPP - 	write 1 bit per pixel paletted bitmap
	//					bitmap data in array at pixels, 4 bits per pixel
	//					color palette data in array at palette
	//					width must be at least 8 pixels
	void writeRect1BPP(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t *pixels, const uint16_t * palette );

		// writeRectNBPP - 	write N(1, 2, 4, 8) bit per pixel paletted bitmap
	//					bitmap data in array at pixels
	//  Currently writeRect1BPP, writeRect2BPP, writeRect4BPP use this to do all of the work. 
	// 
	void writeRectNBPP(int16_t x, int16_t y, int16_t w, int16_t h,  uint8_t bits_per_pixel, 
		const uint8_t *pixels, const uint16_t * palette );
	
	// from Adafruit_GFX.h
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
	void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
	void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
	void setCursor(int16_t x, int16_t y);
    void getCursor(int16_t *x, int16_t *y);
	void setTextColor(uint16_t c);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextSize(uint8_t s);
	uint8_t getTextSize();
	void setTextWrap(boolean w);
	boolean getTextWrap();
	virtual size_t write(uint8_t);
	int16_t width(void)  { return _width; }
	int16_t height(void) { return _height; }
	uint8_t getRotation(void);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	int16_t getCursorX(void) const { return cursor_x; }
	int16_t getCursorY(void) const { return cursor_y; }
	void setFont(const ILI9488_t3_font_t &f) { font = &f; }
	void setFontAdafruit(void) { font = NULL; }
	void drawFontChar(unsigned int c);
	int16_t strPixelLen(char * str);
	void write16BitColor(uint16_t color, bool last_pixel=false);
	
	// setOrigin sets an offset in display pixels where drawing to (0,0) will appear
	// for example: setOrigin(10,10); drawPixel(5,5); will cause a pixel to be drawn at hardware pixel (15,15)
	void setOrigin(int16_t x = 0, int16_t y = 0) { 
		_originx = x; _originy = y; 
		//if (Serial) Serial.printf("Set Origin %d %d\n", x, y);
		updateDisplayClip();
	}
	void getOrigin(int16_t* x, int16_t* y) { *x = _originx; *y = _originy; }

	// setClipRect() sets a clipping rectangle (relative to any set origin) for drawing to be limited to.
	// Drawing is also restricted to the bounds of the display

	void setClipRect(int16_t x1, int16_t y1, int16_t w, int16_t h) 
		{ _clipx1 = x1; _clipy1 = y1; _clipx2 = x1+w; _clipy2 = y1+h; 
			//if (Serial) Serial.printf("Set clip Rect %d %d %d %d\n", x1, y1, w, h);
			updateDisplayClip();
		}
	void setClipRect() {
			 _clipx1 = 0; _clipy1 = 0; _clipx2 = _width; _clipy2 = _height; 
			//if (Serial) Serial.printf("clear clip Rect\n");
			 updateDisplayClip(); 
		}
	
	
	// added support for drawing strings/numbers/floats with centering
	// modified from tft_ili9488_ESP github library
	// Handle numbers
	int16_t  drawNumber(long long_num,int poX, int poY);
	int16_t  drawFloat(float floatNumber,int decimal,int poX, int poY);   
	// Handle char arrays
	int16_t drawString(const String& string, int poX, int poY);
	int16_t drawString1(char string[], int16_t len, int poX, int poY);

	void setTextDatum(uint8_t datum);
	
	// added support for scrolling text area
	// https://github.com/vitormhenrique/ILI9488_t3
	// Discussion regarding this optimized version:
    //http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9488-%28320x240-TFT-color-display%29-library
	//	
	void setScrollTextArea(int16_t x, int16_t y, int16_t w, int16_t h);
	void setScrollBackgroundColor(uint16_t color);
	void enableScroll(void);
	void disableScroll(void);
	void scrollTextArea(uint8_t scrollSize);
	void resetScrollBackgroundColor(uint16_t color);
	
	// added support to use optional Frame buffer
	void	setFrameBuffer(uint8_t *frame_buffer);
	uint8_t *getFrameBuffer() {return _pfbtft;}
	uint8_t useFrameBuffer(boolean b);		// use the frame buffer?  First call will allocate
	void	freeFrameBuffer(void);			// explicit call to release the buffer
	void	updateScreen(void);				// call to say update the screen now. 

	// Support for user to set Pallet. 
	void	setPallet(uint16_t *pal, uint16_t count);	// <= 256
	void	colorsArePalletIndex(boolean b) {_colors_are_index = b;}
	boolean	colorsArePalletIndex() {return _colors_are_index;}
	
	// probably not called directly... 
	uint8_t doActualConvertColorToIndex(uint16_t color);  

	inline uint8_t mapColorToPalletIndex(uint16_t color) 
		{
			if (_pallet && _colors_are_index) return (uint8_t)color;
			return doActualConvertColorToIndex(color);		
		}
		
	bool	updateScreenAsync(bool update_cont = false);	// call to say update the screen optinoally turn into continuous mode. 
	void	waitUpdateAsyncComplete(void);
	void	endUpdateAsync();			 // Turn of the continueous mode fla
	void	dumpDMASettings();
	#ifdef ENABLE_ILI9488_FRAMEBUFFER
	uint32_t frameCount() {return _dma_frame_count; }
	boolean	asyncUpdateActive(void)  {return (_dma_state & ILI9488_DMA_ACTIVE);}
	void	initDMASettings(void);
	#else
	uint32_t frameCount() {return 0; }
	boolean	asyncUpdateActive(void)  {return false;}
	uint32_t frameCount() {return 0; }
	#endif


 protected:
    SPIClass                *spi_port;
    uint32_t                _spi_port_memorymap = 0;
#if defined(KINETISK)
 	//KINETISK_SPI_t *_pkinetisk_spi;
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
	//inline IMXRT_LPSPI_t & port() { return (*(IMXRT_LPSPI_t *)0x403A0000); }
	//IMXRT_LPSPI_t *_pimxrt_spi;
#elif defined(KINETISL)
 	//KINETISL_SPI_t *_pkinetisl_spi;
#endif
	int16_t _width, _height; // Display w/h as modified by current rotation
	int16_t  cursor_x, cursor_y;
	
	int16_t  _clipx1, _clipy1, _clipx2, _clipy2;
	int16_t  _originx, _originy;
	int16_t  _displayclipx1, _displayclipy1, _displayclipx2, _displayclipy2;
	bool _invisible = false; 
	bool _standard = true; // no bounding rectangle or origin set. 

	inline void updateDisplayClip() {
		_displayclipx1 = max(0,min(_clipx1+_originx,width()));
		_displayclipx2 = max(0,min(_clipx2+_originx,width()));

		_displayclipy1 = max(0,min(_clipy1+_originy,height()));
		_displayclipy2 = max(0,min(_clipy2+_originy,height()));
		_invisible = (_displayclipx1 == _displayclipx2 || _displayclipy1 == _displayclipy2);
		_standard =  (_displayclipx1 == 0) && (_displayclipx2 == _width) && (_displayclipy1 == 0) && (_displayclipy2 == _height);
		if (Serial) {
			//Serial.printf("UDC (%d %d)-(%d %d) %d %d\n", _displayclipx1, _displayclipy1, _displayclipx2, 
			//	_displayclipy2, _invisible, _standard);

		}
	}
	
	uint16_t textcolor, textbgcolor, scrollbgcolor;
	uint8_t textsize, rotation,textdatum;
	boolean wrap; // If set, 'wrap' text at right edge of display
	const ILI9488_t3_font_t *font;

	uint32_t padX;
	int16_t scroll_x, scroll_y, scroll_width, scroll_height;
	boolean scrollEnable,isWritingScrollArea; // If set, 'wrap' text at right edge of display

  	uint8_t  _rst;
  	uint8_t _cs, _dc;
	uint8_t pcs_data, pcs_command;
	uint8_t _miso, _mosi, _sclk;
	// add support to allow only one hardware CS (used for dc)
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
    uint32_t _cspinmask;
    volatile uint32_t *_csport;
    uint32_t _spi_tcr_current;
    uint32_t _dcpinmask;
    uint8_t _pending_rx_count;
    volatile uint32_t *_dcport;
#else
    uint8_t _cspinmask;
    volatile uint8_t *_csport;
#endif

//#ifdef ENABLE_ILI9488_FRAMEBUFFER
    // Add support for optional frame buffer
    uint8_t		*_pfbtft;						// Optional Frame buffer 
    uint8_t		_use_fbtft;						// Are we in frame buffer mode?
    uint8_t		*_we_allocated_buffer;			// We allocated the buffer; 

    uint16_t	*_pallet;						// Support for user to set Pallet. 
    uint16_t	_pallet_size;					// How big is the pallet
    uint16_t	_pallet_count;					// how many items are in it...
    boolean		_colors_are_index;				// are the values passed in index or color?

    // Add DMA support. 
	static  ILI9488_t3 		*_dmaActiveDisplay;  // Use pointer to this as a way to get back to object...
	static volatile uint8_t  	_dma_state;  		// DMA status
	static volatile uint32_t	_dma_frame_count;	// Can return a frame count...
	#if defined(__MK66FX1M0__) 
	static DMASetting 	_dmasettings[3];
	static DMAChannel  	_dmatx;
	#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)  // Teensy 4.x
	// Going to try it similar to T4.
	static DMASetting 	_dmasettings[2];
	static DMAChannel  	_dmatx;
	uint32_t 			_spi_fcr_save;		// save away previous FCR register value
	#else
	#endif	
	static void dmaInterrupt(void);
	void process_dma_interrupt(void);
	bool fillDMApixelBuffer(uint8_t *buffer_ptr);

	enum {DMA_PIXELS_OUTPUT_PER_DMA=80};  // How many pixels at a time?
	uint8_t _dma_pixel_buffer0[DMA_PIXELS_OUTPUT_PER_DMA*3] __attribute__ ((aligned(4)));
	uint8_t _dma_pixel_buffer1[DMA_PIXELS_OUTPUT_PER_DMA*3] __attribute__ ((aligned(4)));
	static volatile uint32_t _dma_pixel_index;
	static volatile uint16_t	_dma_sub_frame_count;	// Can return a frame count...


//#endif


//----------------------------------------------------------------------
// Processor Specific stuff

	void DIRECT_WRITE_LOW(volatile uint32_t * base, uint32_t mask);
	void DIRECT_WRITE_HIGH(volatile uint32_t * base, uint32_t mask);
	void writecommand_cont(uint8_t c);
	void writedata8_cont(uint8_t c); 
	void writedata16_cont(uint16_t d);
	void writecommand_last(uint8_t c);
	void writedata8_last(uint8_t c);
	void writedata16_last(uint16_t d);
	void beginSPITransaction(uint32_t clock = ILI9488_SPICLOCK);
	void endSPITransaction();
	void maybeUpdateTCR(uint32_t requested_tcr_state);

	void waitFifoNotFull(void);	
	
	void waitFifoEmpty(void);
	void waitTransmitComplete(void);
	void waitTransmitComplete(uint32_t mcr);
	
	void setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void HLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void VLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void Pixel(int16_t x, int16_t y, uint16_t color);


	void drawFontBits(bool opaque, uint32_t bits, uint32_t numbits, int32_t x, int32_t y, uint32_t repeat);};

#ifndef swap
#define swap(a, b) { typeof(a) t = a; a = b; b = t; }
#endif

// To avoid conflict when also using Adafruit_GFX or any Adafruit library
// which depends on Adafruit_GFX, #include the Adafruit library *BEFORE*
// you #include ILI9488_t3.h.
#ifndef _ADAFRUIT_GFX_H
class Adafruit_GFX_Button {
public:
	Adafruit_GFX_Button(void) { _gfx = NULL; }
	void initButton(ILI9488_t3 *gfx, int16_t x, int16_t y,
		uint8_t w, uint8_t h,
		uint16_t outline, uint16_t fill, uint16_t textcolor,
		const char *label, uint8_t textsize);
	void drawButton(bool inverted = false);
	bool contains(int16_t x, int16_t y);
	void press(boolean p) {
		laststate = currstate;
		currstate = p;
	}
	bool isPressed() { return currstate; }
	bool justPressed() { return (currstate && !laststate); }
	bool justReleased() { return (!currstate && laststate); }
private:
	ILI9488_t3 *_gfx;
	int16_t _x, _y;
	uint16_t _w, _h;
	uint8_t _textsize;
	uint16_t _outlinecolor, _fillcolor, _textcolor;
	char _label[10];
	boolean currstate, laststate;
};
#endif

#endif // __cplusplus


#endif
