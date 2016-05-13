//#ifndef _NOEUD_H_
//#define _NOEUD_H_
//
//#include "object.h"
//#include "vect.h"
//
//struct SBBOX {
//    float rayon_sphere;
//    Vect centre;
//};
//
//class BVHNoeud{
//    SBBOX sbbox;
//    Object object;
//    BVHNoeud *left;
//    BVHNoeud *right;
//
//
//
//    BVHNoeud(Object *object);
//    BVHNoeud(vector<Object *> &liste);
////    virtual ~BVHNoeud();
//
////    Intersection GetIntersection(const Line &l) const;
////    bool DoesIntersect(const Line &l) const;
////    double GetIntersectionT(const Line &l) const;
//
//    // Get the sphere bounding box.
//    SBBox getSBBox() const { return sbbox; }
//
//    // Interior node: triangle = NULL.
//    // Leaf node: triangle != NULL, left = right = NULL.
//
//
//
//
//
//
//    SBBOX FusionnerSBBOX(const SBBOX &b1, const SBBOX &b2) {
//        SBBOX b;
//        Vect sphereMin = Vect::Min(b1.centre, b2.centre);
//        Vect sphereMax = Vect::Max(b1.centre, b2.centre);
//        Vect centre = sphereMin.vectAdd(sphereMax).vectMult(0.5);
//
//        double r = centre.distance(sphereMax);
//
//        b.centre = centre;
//        b.rayon_sphere = r;
//        return b;
//    }
//
//
//
//
//
//    BVHNoeud BVHNoeud(Object *object) {
//        // Constructiion d'une feuille
//        this->sbbox = object->getSBBOX();
//        this->object = object;
//        this->left = this->right = NULL;
//    }
//
//
//    // Init a bvh tree.
//    BVHNoeud::BVHNoeud(std::vector<Object *> &objs) {
//        int num = (int)objs.size();
//        if (num == 0) {
//            this->object = NULL;
//            this->left = this->right = NULL;
//        } else if (num == 1) {
//            this->object = objs.at(0);
//            this->left = this->right = NULL;
//            this->sbbox = this->object.getSBBOX();
//        } else if (num == 2) {
//            this->object = NULL;
//            this->left = new BVHNoeud(objs.at(0));
//            this->right = new BVHNoeud(objs.at(1));
//            this->sbbox = FusionnerSBBOX(this->left->sbbox, this->right->sbbox);
//        } else {
//            SBBOX bound;
//            for (int i = 0; i < num; i++) {
//                bound = SBBOX::FusionnerSBBOX(bound, objs[i]->GetBBox());
//            }
//            int id = bound.GetMaxExtentId();
//            switch (id) {
//                case 0:
//                    std::sort(objs.begin(), objs.end(), CompareX);
//                    break;
//                case 1:
//                    std::sort(objs.begin(), objs.end(), CompareY);
//                    break;
//                case 2:
//                default:
//                    std::sort(objs.begin(), objs.end(), CompareZ);
//                    break;
//            }
//            vector<Object *> lobjs;
//            vector<Object *> robjs;
//            for (int i = 0; i < num / 2; i++)
//                lobjs.push_back(objs[i]);
//            for (int i = num / 2; i < num; i++)
//                robjs.push_back(objs[i]);
//
//            this->triangle = NULL;
//            this->left = new BVHNoeud(lobjs);
//            this->right = new BVHNoeud(robjs);
//            this->bbox = SBBOX::Combine(this->left->bbox, this->right->bbox);
//        }
//    }
//
//
//
//
//
//};
//
//
//
//
//
//
//
//
//bool CompareX(Object* t1, Object* t2) {
//    Vector3 c1 = t1->GetBBox().GetCenter();
//    Vector3 c2 = t2->GetBBox().GetCenter();
//    return c1.x < c2.x;
//}
//
//bool CompareY(Object* t1, Object* t2) {
//    Vector3 c1 = t1->GetBBox().GetCenter();
//    Vector3 c2 = t2->GetBBox().GetCenter();
//    return c1.y < c2.y;
//}
//
//bool CompareZ(Object* t1, Object* t2) {
//    Vector3 c1 = t1->GetBBox().GetCenter();
//    Vector3 c2 = t2->GetBBox().GetCenter();
//    return c1.z < c2.z;
//}
//
//
//
//#endif