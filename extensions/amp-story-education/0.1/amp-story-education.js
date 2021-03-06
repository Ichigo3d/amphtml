/**
 * Copyright 2020 The AMP HTML Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS-IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import {CSS} from '../../../build/amp-story-education-0.1.css';
import {Layout} from '../../../src/layout';
import {htmlFor} from '../../../src/static-template';

/**
 * Generates the amp-story-education template.
 * @param {!Element} element
 * @return {!Element}
 */
const buildFooTemplate = element => {
  const html = htmlFor(element);
  return html`
    <div class="i-amphtml-story-education"></div>
  `;
};

export class AmpStoryEducation extends AMP.BaseElement {
  /** @param {!AmpElement} element */
  constructor(element) {
    super(element);
  }

  /** @override */
  buildCallback() {
    const template = buildFooTemplate(this.element);
    this.element.appendChild(template);
  }

  /** @override */
  isLayoutSupported(layout) {
    return layout === Layout.CONTAINER;
  }
}

AMP.extension('amp-story-education', '0.1', AMP => {
  AMP.registerElement('amp-story-education', AmpStoryEducation, CSS);
});
