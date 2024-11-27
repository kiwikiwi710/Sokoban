
// SokobanDoc.h : interface of the CSokobanDoc class
//


#pragma once


class CSokobanDoc : public CDocument
{
protected: // create from serialization only
	CSokobanDoc() noexcept;
	DECLARE_DYNCREATE(CSokobanDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSokobanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	void Readmap(int);
	int level,A,B,D,x,y,r,c,step,flag,x1,y1;
	char map[MAX][MAX];
};
