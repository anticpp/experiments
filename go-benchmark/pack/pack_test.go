package pack

import (
	"testing"
)

func TestCheck(t *testing.T) {
	p := New(100)
	if !p.Check(){
		t.Errorf("Check fail")
	}
}

func TestData(t *testing.T) {
	p := New(100)
	data :=  p.Data()
	if len(data)!=100 {
		t.Errorf("Data should be 100 bytes")
	}
}

func TestRaw(t *testing.T) {
	p := New(100)
	raw :=  p.Raw()
	if len(raw)!=104 {
		t.Errorf("Data should be 104 bytes")
	}
}
