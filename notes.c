
	BRect horizontalScrollbarFrame(Bounds());
	horizontalScrollbarFrame.left = horizontalScrollbarFrame.right - B_V_SCROLL_BAR_WIDTH;
	fHorizontalScrollbar = new BScrollBar(Bounds(), "horizontalScrollbar", this, 0, 10, B_HORIZONTAL);

	BRect verticalScrollbarFrame(Bounds());
	verticalScrollbarFrame.top = verticalScrollbarFrame.bottom - B_H_SCROLL_BAR_HEIGHT;
	fVerticalScrollbar = new BScrollBar(Bounds(), "verticalScrollbar", this, 0, 10, B_VERTICAL);


****************************


/// TODO: add the ability to layout dinamically while dragging over
/// rethink the interface a la posespatialcache
///
/// 	virtual	void		AddPose(BPose* pose) = 0;	// maybe with a layout_now bool, oui c'est necessaire
/// 	virtual	void		AddPoses(PoseList* list) = 0;
///		virtual	void		RemovePose(BPose* pose) = 0;
///		virtual	void		RemoveAllPoses() = 0;

///		virtual void		LayoutAllPoses() = 0;
///						 ?LayoutVisiblePoses() = 0;

// le pose layouter modifie et enregistre la position de chaque pose ce qui permet d'emuler le mode liste.
// ainsi chaque layouter na pas besoin de reorganiser les icones deja placés
// en ce moment, le mode liste a une position stockée differement du mode icone
// on pourrait aussi stocker le positionement de differents tris afin d'avoir une reponse immediate en changeant l'ordre

// maybe have an active flag so that adding poses to an unactive layout doesn't trigger calculations

// c'est en fait juste une memoire et un trieur de positions selon certains criteres
// on peut soit placer/trier a la volee, soit tout refaire,  lorsque on est inactif on note juste les poses nouvelles a traite a la prochaine activation

//mais alors chaque layouter aura un spatial cache puisque un jeu de positions differents

//le plus rapide pour que chaque pose ait une position dans chaque mode est de stocker plusieurs positions dans BPose
//une position par layouter

// donc
/*class PoseLayouter2 {
public:
	virtual				~PoseLayouter2();

	virtual	void		AddPose(BPose* pose, bool updateNow = false) = 0;	// maybe with a layout_now bool, oui c'est necessaire
	virtual	void		AddPoses(PoseList* list, bool updateNow = false) = 0;
	virtual	void		RemovePose(BPose* pose) = 0;
	virtual	void		RemovePoses(BPoseList* list) = 0;
	virtual	void		RemoveAllPoses() = 0;

	virtual void		LayoutAllPoses() = 0;
	virtual void		Activate() // Work()

						UpdateAllPoses(); // change la position de chaque pose, si differente de celle stockée dans pose->position
						UpdatePosesInRect(const Brect& rect); // si c'est rapide pas besoin d'updater ce qu'on ne voit pas
						// ca veut dire que les positions sont dans un spatial cache

	// voir si on peut implementer le list mode (cell layout)

};

*/