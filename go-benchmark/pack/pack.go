package pack

var (
	magicNum = 0xedb88320
)

type Pack struct {
	buf []byte
}

func New(size int) *Pack {
	p := &Pack{
			buf: make([]byte, size+4),
	}
	p.buf[0] = byte( (magicNum >> 24) & 0xFF )
	p.buf[1] = byte( (magicNum >> 16) & 0xFF )
	p.buf[2] = byte( (magicNum >> 8) & 0xFF )
	p.buf[3] = byte( magicNum & 0xFF )
	return p
}

func (p *Pack) Check() bool {

	cs := ( int(p.buf[0]) << 24 ) +
		( int(p.buf[1]) << 16 ) +
		( int(p.buf[2]) << 8 ) +
		( int(p.buf[3]) )

	return cs==magicNum
}

// Return usable data.
func (p *Pack) Data() []byte {
	return p.buf[4:]
}

// Return all data, including 4 bytes check magic.
func (p *Pack) Raw() []byte {
	return p.buf
}
