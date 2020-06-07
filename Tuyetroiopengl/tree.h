class Tree {
public:
	//variables
	float positionX, positionY, positionZ;
	float height;
	float base;
	float size;
	bool animation;
	int treeBranch;
	int treeLevel;
	GLuint makeaTree;

	//functions
	Tree(float positionX, float PositionY, float positionZ, float height, float base, float size);
	void render();
	void setTreeLevel(int level);
	void setTreeBranch(int branch);
	void setLeafStyle(int num);
	void setLeafColor(float R, float G, float B);
	void setTrunkColor(float R, float G, float B);
	void setRange(float range);
	void enableRandomAngle(bool state);
	void enableRandomColor(bool state);
	void buildTree();


private:
	//variables
	float angle;
	int leafStyle;
	float range;
	bool randomColor;
	float leafRed, leafGreen, leafBlue;
	float trunkRed, trunkGreen, trunkBlue;
	bool randomAngle;
	float staticAngle;

	//functions
	void init();
	void makeCylinder(float height, float base);
	void makeTree(float height, float base, float range);
};